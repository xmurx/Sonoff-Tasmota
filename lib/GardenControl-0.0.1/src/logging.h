#ifndef _LOGGING_H_
#define _LOGGING_H_

class Log
{
public:    
    template<typename... Args>
    static void Debug( const char* format, Args... args );

    template<typename... Args>
    static void Info(  const char* format, Args... args );

    template<typename... Args>
    static void Error( const char* format, Args... args );
};    

#endif