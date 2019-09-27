#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include <Arduino.h>
#include "sonoff.h"
#include "logging.h"
#include "x_support.h"

/*********************************************************************************************\
 * Logging helper functions
\*********************************************************************************************/

/**
 * @brief print Debug log
 * 
 * @param format - format string
 * @param args - variable parameter list
 */

template<typename... Args>
void Log::Debug( const char* format, Args... args)
{
  AddLog_P2(LOG_LEVEL_DEBUG, format, args...);
}

/**
 * @brief print Info log
 * 
 * @param format - format string
 * @param args - variable parameter list
 */
template<typename... Args>
void Log::Info( const char* format, Args... args)
{
  AddLog_P2(LOG_LEVEL_INFO, format, args...);
}

/**
 * @brief print Error log
 * 
 * @param format - format string
 * @param ... - variable parameter list
 */
template<typename... Args>
void Log::Error( const char* format, Args... args)
{
  AddLog_P2(LOG_LEVEL_ERROR, format, args...);
}

/*********************************************************************************************\
 * class MQTTResponse
\*********************************************************************************************/

/**
 * @brief AppendP - appends formated data to the MQTT send buffer 
 * 
 * @param formatP - format string localted in internal flash
 * @param args - variable parameter list
 */
template<typename... Args>
void MQTTResponse::AppendP( PGM_P formatP, Args... args )
{
  ResponseAppend_P(formatP, args...);
}

/**
 * @brief Appends formated data to the MQTT send buffer
 * 
 * @param format - format string located in RAM
 * @param args - variable parameter list
 */
template<typename... Args>
void MQTTResponse::Append( const char* format, Args... args )
{
  ResponseAppend_P(format, args...);
}

/**
 * @brief returns to base address to internal mqtt buffer
 * 
 * @tparam Args 
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