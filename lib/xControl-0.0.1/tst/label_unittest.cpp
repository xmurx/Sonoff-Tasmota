#include "gtest/gtest.h"

#include "label.h"
#include "mock_renderer.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;

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