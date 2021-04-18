#ifndef __MOCK_RENDERER_H__
#define __MOCK_RENDERER_H__

#include "renderer.h"
class MockRenderer : public Renderer
{
public:
  MOCK_METHOD(void, setCursor, (int16_t x, int16_t y), (override));
  MOCK_METHOD(void, clearDisplay, (), (override));
  MOCK_METHOD(void, setTextSize, (uint8_t s), (override));
  MOCK_METHOD(size_t, print, (const char* value), (override));
  MOCK_METHOD(void, Updateframe, (), (override));
  MOCK_METHOD(void, drawBitmap, (int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color), (override));
  MOCK_METHOD(void, drawChar, (int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size), (override));
  MOCK_METHOD(void, drawRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));
  MOCK_METHOD(void, fillRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));
};

#endif