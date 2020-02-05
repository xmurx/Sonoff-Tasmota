#include <chrono>
#include <thread>

#include "mock_tasmota.h"

unsigned long millis();

namespace xControl
{

class A : public TasmotaFixture
{
  public:

};

TEST_F(A, Call_To_Millis)
{
  EXPECT_CALL(*A::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(100));

  using namespace std::chrono_literals;
  unsigned long start = millis();
  std::this_thread::sleep_for(100ms);
  unsigned long end = millis();
  unsigned long difference = end - start;
  EXPECT_TRUE((difference >= 100));
}

} //end of namespace