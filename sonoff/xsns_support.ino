#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include <Arduino.h>
#include "sonoff.h"
#include "logging.h"

/*********************************************************************************************\
 * Logging helper functions
\*********************************************************************************************/

void Log::Debug( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  AddLog(LOG_LEVEL_DEBUG);
  va_end (args);

}

void Log::Info( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  AddLog(LOG_LEVEL_INFO);
  va_end (args);
}

void Log::Error( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  AddLog(LOG_LEVEL_ERROR);
  va_end (args);
}

/*********************************************************************************************\
 * class Response
\*********************************************************************************************/

void Response::AppendP( PGM_P formatP, ... )
{
  size_t len = strlen(Response::Get());
  char* nextDataStart = &mqtt_data[len];

  va_list args;
  va_start (args, formatP);
  vsnprintf_P (nextDataStart,(sizeof(mqtt_data)-len),formatP, args);
  va_end (args);
}

void Response::Append( const char* format, ... )
{
  size_t len = strlen(Response::Get());
  char* nextDataStart = &mqtt_data[len];

  va_list args;
  va_start (args, format);
  vsnprintf_P (nextDataStart,(sizeof(mqtt_data)-len),format, args);
  va_end (args);
}

const char* Response::Get()
{
  return mqtt_data;
}