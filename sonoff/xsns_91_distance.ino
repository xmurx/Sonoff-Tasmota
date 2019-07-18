#include "distance.h"
#include "logging.h"
#include "xsns_support.h"

#ifdef USE_VIRTUAL_DISTANCE_MEASUREMENT

// wrapper for appending formated data 
// to the global tasmota mqtt data

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

// {s}, {m} and {e} are replaced by HTML Table tags by the webserver (xdrv_01_webserver.ino)
// "%s{s}%s " D_DISTANCE "{m}%s %s{e}";
//   |    |        |          |  |unit
//   |    |        |          |sensor value    
//   |    |        | translates sensor typ - in this case distance|abstand 
//   |    | sensor name 
//   | previous string - in this case [mqtt_data] than means all sensors append there values to this buffer
//                       The webserver will shows the given values - MqttData::Append - does not need the previos string (content remains unchanged)
// The interface between sensors and webserver are global accessable variables (mqtt_data)

const char _HTTP_SNS_DISTANCE[] PROGMEM = "{s}%s " D_DISTANCE "{m}%s %s {e}";
static Distance sensor;

#define XSNS_91
bool Xsns91(uint8_t function)
{
  boolean result = true;
  static double _lastSensorValue_ = 0;
  switch (function)
  {
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
    {
      std::string distance = sensor.ToString(_lastSensorValue_);
      WebServer::SendContent( _HTTP_SNS_DISTANCE, sensor.Name().c_str(), distance.c_str(), "cm");
      break;
    }
#endif    
    case FUNC_INIT:
    {
      sensor.Init(360,20,30);
      _lastSensorValue_ = sensor.Read();
      break;
    }
    case FUNC_JSON_APPEND:
    {
      _lastSensorValue_ = sensor.Read();
      std::string distance = sensor.ToString(_lastSensorValue_);
      MQTTResponse::Append( PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%s}"), sensor.Name().c_str(), distance.c_str() );
      if(0 == tele_period)
      {
        DomoticzSensor(DZ_COUNT, (char*)distance.c_str());
      }
      break;
    }
  }
  return result;
}

#endif //USE_DISTANCE_MEASUREMENT