#ifndef __MOCK_TASMOTA_H__
#define __MOCK_TASMOTA_H__

#include "gmock/gmock.h"

namespace xControl
{
  class Time
  {
  public:
    Time() {}
    virtual ~Time() {}

    virtual unsigned long Millis() = 0;
  };

  using namespace testing;
  class TimeMock : public xControl::Time
  {
  public:
    TimeMock() {}
    virtual ~TimeMock() {}

    MOCK_METHOD(unsigned long, Millis, (), (override));
  };

  class TasmotaFixture : public ::testing::Test
  {
  public:
    TasmotaFixture();
    virtual ~TasmotaFixture();

    static std::unique_ptr<TimeMock> _time;
  };
}

#endif