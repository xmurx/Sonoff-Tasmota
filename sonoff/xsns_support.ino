#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include <Arduino.h>
#include "sonoff.h"
#include "logging.h"

/*********************************************************************************************\
 * Logging helper functions
\*********************************************************************************************/

/**
 * @brief print Debug log
 * 
 * @param format - format string
 * @param ... - variable parameter list
 */
void Log::Debug( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  va_end (args);

  AddLog(LOG_LEVEL_DEBUG);
}

/**
 * @brief print Info log
 * 
 * @param format - format string
 * @param ... - variable parameter list
 */
void Log::Info( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  va_end (args);

  AddLog(LOG_LEVEL_INFO);
}

/**
 * @brief print Error log
 * 
 * @param format - format string
 * @param ... - variable parameter list
 */
void Log::Error( const char* format, ...)
{
  va_list args;
  va_start (args, format);
  vsnprintf (log_data, sizeof(log_data),format, args);
  va_end (args);  

  AddLog(LOG_LEVEL_ERROR);
}

/*********************************************************************************************\
 * class MQTTResponse
\*********************************************************************************************/

/**
 * @brief AppendP - appends formated data to the MQTT send buffer 
 * 
 * @param formatP - format string localted in internal flash
 * @param ... - variable parameter list
 */
void MQTTResponse::AppendP( PGM_P formatP, ... )
{
  size_t len = strlen(MQTTResponse::Get());
  char* nextDataStart = &mqtt_data[len];

  va_list args;
  va_start (args, formatP);
  vsnprintf_P (nextDataStart,(sizeof(mqtt_data)-len),formatP, args);
  va_end (args);
}

/**
 * @brief Appends formated data to the MQTT send buffer
 * 
 * @param format - format string located in RAM
 * @param ... - variable parameter list
 */
void MQTTResponse::Append( const char* format, ... )
{
  size_t len = strlen(MQTTResponse::Get());
  char* nextDataStart = &mqtt_data[len];

  va_list args;
  va_start (args, format);
  vsnprintf_P (nextDataStart,(sizeof(mqtt_data)-len),format, args);
  va_end (args);
}

/**
 * @brief Get - returns the pointer to the MQTT-buffer for read access
 * 
 * @return const char* 
 */
const char* MQTTResponse::Get()
{
  return mqtt_data;
}

/*********************************************************************************************\
 * class WebServer
\*********************************************************************************************/

/**
 * @brief SendContent - sends the data over Webserver interface
 * 
 * @tparam Args - variable template paramenter list
 * @param format - format string (see printf) 
 * @param args  - format arguments 
 */
template<typename... Args>
void WebServer::SendContent(const char* format, Args... args)
{
    WSContentSend_P(format, args...);
}