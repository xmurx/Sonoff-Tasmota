
#include "ArduinoJson.h"

#include "gc_support.h"
#include "logging.h"
#include "images.h"
#include "view.h"

#ifdef USE_X_VIEW_DRIVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90 90

namespace View
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 120;
  StaticJsonBuffer<capacity> jsonBuffer;
  ViewData data;

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
} // end of namespace XView

bool Xdrv90(uint8_t function)
{
  static View::SSD1306View view(renderer);
  bool result = true;
  static unsigned long start = millis();
  switch (function)
  {
    case FUNC_INIT:
    {
      view.Execute(View::SSD1306View::Command::Init, View::data);
      break;
    }
    case FUNC_EVERY_250_MSECOND:
    {
      static uint32_t delay = 0;
      static bool splashFinnished = false;

      if(delay <= 8) { delay++; }
      else
      {
        splashFinnished = true;
        if(renderer != NULL)
        {
          renderer->clearDisplay();
          renderer->setTextFont(0);
          renderer->setTextSize(2);
          renderer->setCursor(0,0);
          renderer->Updateframe();
        }
      }

      if(splashFinnished)
      {
        uint8_t snsIndex = 0;
        do
        {
          MQTTResponse::Clear();
          MQTTResponse::AppendTime();
          XsnsNextCall(FUNC_JSON_APPEND, snsIndex);
          MQTTResponse::AppendEnd();

          bool ret = View::ExtractValues(View::data);
          if(ret==true)
          {
            unsigned long timeDiff = millis() - start;
            if(renderer != NULL)
            {
              renderer->setCursor(12,10);
              renderer->printf( "%d mm", View::data.Distance());
              renderer->Updateframe();
            }
          }
        } while (snsIndex != 0);
      }
      break;
    }
    case FUNC_EVERY_SECOND:
    {
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