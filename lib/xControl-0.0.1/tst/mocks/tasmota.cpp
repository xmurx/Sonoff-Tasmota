#include "tasmota.h"
#include "common.h"
#include "logging.h"
#include "stdarg.h"
unsigned long millis()
{
  /*
  #include <chrono>
  using namespace std::chrono;
  static auto start = system_clock::now();
  auto now = time_point_cast<milliseconds>(system_clock::now());
  auto difference = duration_cast<milliseconds>(now - start);
  return difference.count();
  */
  return xControl::TasmotaFixture::_time->Millis();
}

void ExecuteCommand(const char* cmnd, uint32_t source)
{
}

namespace xControl
{
  std::unique_ptr<TimeMock> TasmotaFixture::_time;
  std::unique_ptr<CommandMock> TasmotaFixture::_command;

  TasmotaFixture::TasmotaFixture()
  {
    _time.reset(new testing::NiceMock<TimeMock>());
    _command.reset(new testing::NiceMock<CommandMock>());
  }

  TasmotaFixture::~TasmotaFixture()
  {
    _time.reset();
    _command.reset();
  }

  void LogWrapper(LogLevel level, PGM_P formatP, ...)
  {
    va_list argptr;
    va_start(argptr, formatP);
    vfprintf(stdout, formatP, argptr);
    va_end(argptr);
  }
} // namespace xControl