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
    : distance(0),
      temperature(0.0),
      pressure(0),
      humidity(0.0)
    {
    }

    int distance;
    int temperature;
    int humidity;
    int pressure;
  };

  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 120;
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

    Log::Debug( "xView:  %s", MQTTResponse::Get() );
    JsonObject& root = jsonBuffer.parseObject(MQTTResponse::Get());
    if(root.success())
    {
      /*
      const JsonObject& BME280 = root["BME280"];
      if( BME280.success())
      {
        Log::Debug( "xView: BME280 success...");
        data.temperature         = BME280["Temperature"]; 
        data.humidity            = BME280["Humidity"];
        data.pressure            = BME280["Pressure"];
      }

      const JsonObject& VL53L0X = root["VL53L0X"];
      if(VL53L0X.success())
      {
        Log::Debug( "xView: VL53L0X success...");
        data.distance = VL53L0X["Distance"];
      }
      */
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
    case FUNC_EVERY_100_MSECOND:
    {
      static uint8_t snsIndex = 0;
      MQTTResponse::Set(" ");
      MQTTResponse::AppendTime();
      XsnsNextCall(FUNC_JSON_APPEND, snsIndex);
      MQTTResponse::AppendEnd();
      
      bool ret = XView::ExtractValues(XView::data);
      if(ret==true)
      {
        Log::Debug("Temp: %d, Hum: %d, Press: %d, Distance: %d",
                    XView::data.temperature,
                    XView::data.humidity,
                    XView::data.pressure,
                    XView::data.distance );
      }
      else
      {
        //Log::Debug("xview: extract  values from MQTT failed...");
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