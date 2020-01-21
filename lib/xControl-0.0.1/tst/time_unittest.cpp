
#include "gtest/gtest.h"
#include <chrono>
#include <thread>

unsigned long millis();

TEST(MillisTest, Call_To_Millis)
{
  using namespace std::chrono_literals;
  unsigned long start = millis();
  std::this_thread::sleep_for(100ms);
  unsigned long end = millis();
  unsigned long difference = end - start;
  EXPECT_TRUE((difference >= 100));
}