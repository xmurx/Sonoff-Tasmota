#ifndef _XSNS_HELPER_H_
#define _XSNS_HELPER_H_

/**
 * @brief class MQTTresponse encapsultes the access of MQTT data handling
 * 
 */
class MQTTResponse
{
public:
  template<typename... Args>
  static void Set(const char *format, Args... args);

  template<typename... Args>
  static void Append(const char *format, Args... args);

  template<typename... Args>
  static void AppendP(PGM_P formatP, Args... args);
  
  static const char* Get();
  static void AppendEnd();
  static void AppendTime();
  static bool Empty();
  static size_t Size();
  static void Replace(uint32_t position, char character );
};

/**
 * @brief class WebServer encapsulates the access of Webserver data handling
 * 
 */
class WebServer
{
public:

  template<typename... Args>
  static void SendContent(const char* format, Args... args);

};

#endif