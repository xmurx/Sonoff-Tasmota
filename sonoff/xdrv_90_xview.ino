#include "ArduinoJson.h"

#include "x_support.h"
#include "logging.h"
#include "images.h"
#include "view.h"

#ifdef USE_X_VIEW_DRIVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90 90

namespace xControl
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 120;
  StaticJsonBuffer<capacity> jsonBuffer;
  ViewData data;
  StateControl<xControl::State> stateControl;

  /**
   * @brief 
   * 
   * @param data - struct for holdung the extracted data
   * @return true - if extraction of data was successful
   * @return false - if extraction of data fails
   */
  bool ExtractValues( ViewData& data )
  {
    jsonBuffer.clear();
    bool result = false;
    JsonObject& root = jsonBuffer.parseObject(MQTTResponse::Get());
    if(root.success())
    {
      const JsonObject& BME280 = root["BME280"];
      if(BME280.success())
      {
        data.SetTemperature(BME280["Temperature"]);
        data.SetHumidity(BME280["Humidity"]);
        data.SetPressure(BME280["Pressure"]);
        result = true;
      }

      const JsonObject& VL53L0X = root["VL53L0X"];
      if(VL53L0X.success())
      {
        data.SetDistance(VL53L0X["Distance"]);
        result = true;
      }
    }
    return result;
  }
} // end of namespace xControl

bool Xdrv90(uint8_t function)
{
  static xControl::SSD1306View view;
  bool result = true;

  switch (function)
  {
    case FUNC_INIT:
    {
      view.Init(renderer, &xControl::stateControl, Settings.display_width, Settings.display_height);
      break;
    }
    case FUNC_EVERY_250_MSECOND:
    {
      uint8_t snsIndex = 0;
      do
      {
        MQTTResponse::Clear();
        MQTTResponse::AppendTime();
        XsnsNextCall(FUNC_JSON_APPEND, snsIndex);
        MQTTResponse::AppendEnd();
        
        xControl::ExtractValues(xControl::data);
      } while (snsIndex != 0);

      //display determined data 
      view.Show(xControl::data);
      break;
    }
    case FUNC_EVERY_SECOND:
    case FUNC_SHOW_SENSOR:
    {
      break;
    }
  }
  return result;
}

#ifdef TANK_LEVEL_SENSOR
#define XSNS_90 90
static xControl::PercentConverter converter(0, 1000, 10);
bool Xsns90(byte function)
{
  switch (function)
  {
    case FUNC_JSON_APPEND:
    {
      MQTTResponse::AppendP(PSTR(",\"Tank\":{\"Level\":%d}"), (100-converter.ToPercent(xControl::data.Distance())));
      break;
    }
    default:
      break;
  }
}
#endif //TANK_LEVEL_SENSOR

#endif //USE_X_VIEW_ON_SSD1306