#include "gmock/gmock.h"

#include "renderer.h"
#include "label.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;

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
	MOCK_METHOD(void, drawChar, (int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size), (override));
	MOCK_METHOD(void, drawRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));
	MOCK_METHOD(void, fillRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));
};

TEST(LabelTest, SetCursor_correct_centered)
{
  const char txt[] = "1234567890";
  NiceMock<MockRenderer> renderer{};

  //correct position x,y for this txt is: x=4, y =8 
  EXPECT_CALL(renderer, setCursor(Eq(4),Eq(8))).Times(AtLeast(1));
  xControl::Label label{&renderer, 128, 32};
  label.Text(txt);
}

TEST(LabelTest, SetCursor_with_txt_to_long)
{
  const char txt[] = "12345678901";
  NiceMock<MockRenderer> renderer{};

  //incorrect position x,y for this txt is: x=0, y =0 -> txt to long for current display
  EXPECT_CALL(renderer, setCursor(Eq(0),Eq(0))).Times(AtLeast(1));
  xControl::Label label{&renderer, 128, 32};
  label.Text(txt);
}

TEST(LabelTest, SetCursor_and_drawBitmap)
{
  NiceMock<MockRenderer> renderer{};
  xControl::Label label{&renderer, 128, 32};
  xControl::Image icon = xControl::Temperature();
  EXPECT_CALL(renderer, setCursor(Eq(icon.Width()+1),Eq(8))).Times(AtLeast(1));
  label.SetIcon(icon);
  label.Text("Hallo");
}