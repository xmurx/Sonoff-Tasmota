#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "common.h"

namespace xControl
{
  enum LogLevel
  {
    LogLevelNone,
    LogLevelInfo,
    LogLevelError,
    LogLevelDebug
  };
  void LogWrapper(LogLevel level, PGM_P formatP, ...);

  /*********************************************************************************************\
     * Logging helper functions
    \*********************************************************************************************/
  class Logging
  {
  public:
    template <typename... Args>
    static void Debug(const char* format, Args... args)
    {
      LogWrapper(LogLevelDebug, format, args...);
    }

    template <typename... Args>
    static void Info(const char* format, Args... args)
    {
      LogWrapper(LogLevelInfo, format, args...);
    }

    template <typename... Args>
    static void Error(const char* format, Args... args)
    {
      LogWrapper(LogLevelError, format, args...);
    }
  };
} // end of namespace xControl

#endif
