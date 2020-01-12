#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stddef.h>
#include <stdint.h>

class Renderer
{
public:

  Renderer() {}
  virtual ~Renderer() {}

  virtual void setCursor(int16_t x, int16_t y) {}

  virtual void clearDisplay(void) {}

  virtual void setTextSize(uint8_t s) {}

  virtual size_t print(const char* value) { return 0; }

  virtual void Updateframe() { }

  virtual void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {}
};


#endif