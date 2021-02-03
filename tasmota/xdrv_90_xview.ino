#include "x_support.h"
#include "logging.h"
#include "images.h"
#include "view.h"
#include "JsonParser.h"

#ifdef USE_X_VIEW_DRIVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90 90
namespace xControl
{
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
    bool result = false;
    JsonParser parser((char*)MQTTResponse::Get());
    JsonParserObject root = parser.getRootObject();
    if(root)
    {
      JsonParserObject BME280 = root["BME280"].getObject();  
      if(BME280)
      {
        data.SetTemperature(BME280.getFloat("Temperature", 0.0F));
        data.SetHumidity(BME280.getFloat("Humidity", 0.0F));
        data.SetPressure(BME280.getFloat("Pressure", 0.0F));
        result = true;
      }

      JsonParserObject VL53L0X = root["VL53L0X"].getObject();
      if(VL53L0X)
      {
        data.SetDistance(VL53L0X.getInt("Distance", 9999));
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
      ExecuteCommand("TmState 1", SRC_IGNORE);
      ExecuteCommand("TmSend Sensor bootup", SRC_IGNORE);
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