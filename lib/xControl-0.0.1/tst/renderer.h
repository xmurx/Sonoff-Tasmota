#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stddef.h>
#include <stdint.h>

class Renderer
{
public:

  Renderer() {}
  virtual ~Renderer() {}

  virtual void setCursor(int16_t x, int16_t y)
  {
     _x = x;
     _y = y;
  }

  virtual void clearDisplay(void)
  {
  }

  virtual void setTextSize(uint8_t s)
  {
  }

  virtual size_t print(const char* value)
  {
    return strlen(value);
  }

  virtual void Updateframe()
  {
  }

private:

  int16_t _x;
  int16_t _y;
};


#endif