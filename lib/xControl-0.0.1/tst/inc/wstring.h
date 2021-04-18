#ifndef __WSTRING_H__
#define __WSTRING_H__

#include <string>

class String : public std::string
{
public:
  String(const char* str) :
  std::string(str)
  {
  }

  String(float value) :
  std::string(std::to_string(value))
  {
  }

  String(const String& str) :
  std::string(str)
  {
  }

  virtual ~String()
  {
  }
};

#endif