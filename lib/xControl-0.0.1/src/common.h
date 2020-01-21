#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef ESP8266
  #include "Arduino.h"
#else
  #define PROGMEM
  #define PGM_P const char*
#endif

#endif //__COMMON_H__

