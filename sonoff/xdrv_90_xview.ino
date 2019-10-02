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
    float temperature;
    float humidity;
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
    JsonObject& root = jsonBuffer.parseObject(MQTTResponse::Get());
    if(root.success())
    {
      Log::Debug( "XView - root successful");
      const char* Time = root["Time"]; 
      const JsonObject& BME280 = root["BME280"];

      if(BME280.success())
      { 
        Log::Debug( "XView - BME280 successful");
        data.temperature = BME280["Temperature"]; 
        data.humidity = BME280["Humidity"];
        data.pressure = BME280["Pressure"];
      }
      data.distance = root["VL53L0X"]["Distance"];
    }
    else
    {
      return false;
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
    case FUNC_SHOW_SENSOR:
    {
      bool ret = XView::ExtractValues(XView::data);
      if(ret==true)
      {
        Log::Debug("Temp: %f, Hum: %f, Press: %d, Distance: %d",
                    XView::data.temperature,
                    XView::data.humidity,
                    XView::data.pressure,
                    XView::data.distance );
      }
      break;
    }
  }
  return result;
}

#endif //USE_X_VIEW_ON_SSD1306