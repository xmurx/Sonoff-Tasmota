#ifndef _LOGGING_H_
#define _LOGGING_H_

class Log
{
public:    
    static void Debug( const char* format, ... );
    static void Info(  const char* format, ... );
    static void Error( const char* format, ... );
};

#endif