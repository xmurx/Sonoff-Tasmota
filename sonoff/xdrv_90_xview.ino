#include "logging.h"
#include "x_support.h"
#include "ArduinoJson.h"
#include "logging.h"

#ifdef USE_X_VIEW_DRIVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90 90

namespace XView
{
/**
 * @brief xviewData - data for display view
 * @distance - current distance value
 * @temperature - current temperature value
 * @pressure - current air pressure value
 * @humidity - current air humidity value
 */

  class XViewData
  {
  public:

    XViewData()
    : _distance(0),
      _temperature(0.0),
      _pressure(0),
      _humidity(0.0)
    {
    }

    void SetDistance(int distance) {_distance = distance;}
    void SetTemperature(float temperature) { _temperature = temperature;}
    void SetHumidity(float humidity) { _humidity = humidity;}
    void SetPressure(float pressure) { _pressure = pressure;}

    int Distance() {return _distance;}
    float Temperature() {return _temperature;}
    float Humidity() {return _humidity;}
    float Pressure() {return _pressure;}

    void PrintInfo()
    {
      char tempBuffer[8];
      dtostrf(_temperature,3,2,tempBuffer);

      char humBuffer[16];
      dtostrf(_humidity,4,2,humBuffer);

      char presBuffer[16];
      dtostrf(_pressure,4,2,presBuffer);

      Log::Debug("xView - Temp: %s, Hum: %s, Press: %s, Distance: %d",
                    tempBuffer,
                    humBuffer,
                    presBuffer,
                    _distance );
    }

private:

    int _distance;
    float _temperature;
    float _humidity;
    float _pressure;
  };

  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 120;
  StaticJsonBuffer<capacity> jsonBuffer;
  XViewData data;

  /**
   * @brief 
   * 
   * @param data - struct for holdung the extracted data
   * @return true - if extraction of data was successful
   * @return false - if extraction of data fails
   */
  bool ExtractValues( XViewData& data )
  {
    jsonBuffer.clear();
    JsonObject& root = jsonBuffer.parseObject(MQTTResponse::Get());
    if(root.success())
    {
      const JsonObject& BME280 = root["BME280"];
      if(BME280.success())
      {
        Log::Debug( "xView: BME280 success...");
        data.SetTemperature(BME280["Temperature"]);
        data.SetHumidity(BME280["Humidity"]);
        data.SetPressure(BME280["Pressure"]);
      }

      const JsonObject& VL53L0X = root["VL53L0X"];
      if(VL53L0X.success())
      {
        Log::Debug( "xView: VL53L0X success...");
        data.SetDistance(VL53L0X["Distance"]);
      }
    }
    return true;
  }
} // end of namespace XView

bool Xdrv90(uint8_t function)
{
  bool result = true;
  switch (function)
  {
    case FUNC_INIT:
    {
      Log::Debug("XDRV90: Init xview driver");
      break;
    }
    case FUNC_EVERY_250_MSECOND:
    {
      MQTTResponse::Clear();
      MQTTResponse::AppendTime();

      static uint8_t snsIndex = 0;
      XsnsNextCall(FUNC_JSON_APPEND, snsIndex);

      MQTTResponse::AppendEnd();
      bool ret = XView::ExtractValues(XView::data);
      if(ret==true)
      {
        XView::data.PrintInfo();
      }
      else
      {
      }
      break;
    }
    case FUNC_SHOW_SENSOR:
    {
      break;
    }
  }
  return result;
}

#endif //USE_X_VIEW_ON_SSD1306