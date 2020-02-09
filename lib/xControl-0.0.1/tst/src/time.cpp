#include <chrono>
#include <thread>

#include "mocks/tasmota.h"

namespace xControl
{
  TEST_F(TasmotaFixture, Call_To_Millis)
  {
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(100));

    using namespace std::chrono_literals;
    unsigned long start = millis();
    std::this_thread::sleep_for(100ms);
    unsigned long end = millis();
    unsigned long difference = end - start;
    EXPECT_TRUE((difference >= 100));
  }
} //end of namespace