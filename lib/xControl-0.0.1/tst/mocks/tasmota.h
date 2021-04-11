#ifndef __MOCK_TASMOTA_H__
#define __MOCK_TASMOTA_H__

#include "gmock/gmock.h"

extern "C" unsigned long millis();
void ExecuteCommand(const char* cmnd, uint32_t source);
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

  class Command
  {
  public:
    Command() {}
    virtual ~Command() {}

    virtual void ExecuteCommand(const char* cmnd, uint32_t source) = 0;
  };

  class CommandMock : public xControl::Command
  {
    public:
    CommandMock() {}
    virtual ~CommandMock() {}

    MOCK_METHOD(void, ExecuteCommand, (const char* cmnd, uint32_t source), (override));
  };

  class TasmotaFixture : public ::testing::Test
  {
  public:
    TasmotaFixture();
    virtual ~TasmotaFixture();

    static std::unique_ptr<TimeMock> _time;
    static std::unique_ptr<CommandMock> _command;
  };
} // namespace xControl

#endif