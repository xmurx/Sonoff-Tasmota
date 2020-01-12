#ifndef __MOCKRENDERER_H__
#define __MOCKRENDERER_H__

#include "gmock/gmock.h"
#include "renderer.h"

using namespace testing;

class MockRenderer : public Renderer
{
public:

	MOCK_METHOD(void, setCursor, (int16_t x, int16_t y), (override));
	MOCK_METHOD(void, clearDisplay, (), (override));
	MOCK_METHOD(void, setTextSize, (uint8_t s), (override));
	MOCK_METHOD(size_t, print, (const char* value), (override));
	MOCK_METHOD(void, Updateframe, (), (override));
	MOCK_METHOD(void, drawBitmap, (int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color), (override));
};

#endif //__MOCKRENDERER_H__