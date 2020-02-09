#include <chrono>

#include "tasmota.h"

unsigned long millis()
{
  /*
  using namespace std::chrono;
  static auto start = system_clock::now();
  auto now = time_point_cast<milliseconds>(system_clock::now());
  auto difference = duration_cast<milliseconds>(now - start);
  return difference.count();
  */
  return xControl::TasmotaFixture::_time->Millis();
}


namespace xControl
{

  std::unique_ptr<TimeMock> TasmotaFixture::_time;

  TasmotaFixture::TasmotaFixture()
  {
    _time.reset(new testing::NiceMock<TimeMock>());
  }

  TasmotaFixture::~TasmotaFixture()
  {
    _time.reset();
  }
}