#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include <Arduino.h>

#include "tasmota.h"
#include "logging.h"
#include "x_support.h"

namespace xControl
{
  /*********************************************************************************************\
   * Logging helper functions --> adapt loginng to tasmota logging interface
  \*********************************************************************************************/

  void LogWrapper(LogLevel level, PGM_P formatP, ...)
  {
    va_list arg;
    va_start(arg, formatP);
    vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
    va_end(arg);

    uint32_t tasmotaLogLevel = LOG_LEVEL_NONE;
    if(level == LogLevelInfo)
    {
      tasmotaLogLevel = LOG_LEVEL_INFO;
    }
    else if(level == LogLevelError)
    {
      tasmotaLogLevel = LOG_LEVEL_ERROR;
    }
    else if(level == LogLevelDebug)
    {
      tasmotaLogLevel = LOG_LEVEL_DEBUG;
    }
    else
    {
      //LOG_LEVEL_NONE is allready assigned
    }

    AddLog(tasmotaLogLevel);
  }
}

/*********************************************************************************************\
 * class MQTTResponse
\*********************************************************************************************/

/**
 * @brief 
 * 
 * @tparam Args set formated data to the MQTT send buffer 
 * @param formatP - format string localted in internal flash
 * @param args - variable parameter list
 */
template<typename... Args>
void MQTTResponse::Set(PGM_P formatP, Args... args)
{
  Response_P(formatP, args...);
}

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

/**
 * @brief Append curly bracket to the end of the data stream
 * 
 */
void MQTTResponse::AppendEnd()
{
  MQTTResponse::Append(PSTR("}"));
}

/**
 * @brief check if mqtt response is empty
 * 
 * @return true if mqtt data empty
 * @return false if mqtt data not empty
 */
bool MQTTResponse::Empty()
{
  return strlen(MQTTResponse::Get()) > (size_t)0;
}

/**
 * @brief return the data size of the mqtt response
 * 
 * @return size_t current data size (content)
 */
size_t MQTTResponse::Size()
{
  return strlen(MQTTResponse::Get());
}

/**
 * @brief replace one character at position
 * 
 * @param position of character
 * @param character to replace at position 
 */
void MQTTResponse::Replace(uint32_t position, char character)
{
  mqtt_data[position] = character;
}

/**
 * @brief Append time info to mqtt data
 * 
 */
void MQTTResponse::AppendTime()
{
  ResponseAppendTime();
}

/**
 * @brief reset the MQTT buffer
 * 
 */
void MQTTResponse::Clear()
{
  memset(mqtt_data, 0, sizeof(mqtt_data));
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