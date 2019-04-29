#ifndef _XSNS_HELPER_H_
#define _XSNS_HELPER_H_

class Response
{
public:  

  static void AppendP( PGM_P formatP, ... );
  static void Append( const char* format, ... );
  static const char* Get();
};

#endif