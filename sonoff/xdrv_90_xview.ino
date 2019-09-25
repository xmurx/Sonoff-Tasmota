#include "logging.h"
#include "x_support.h"

#ifdef USE_X_VIEW_DRIVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90       90

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
      Log::Debug("XDRV90: show sensor");
      //Log::Debug("XDRV90: %s", MQTTResponse::Get());
      break;
    }
  }
  return result;
}

#endif //USE_X_VIEW_ON_SSD1306