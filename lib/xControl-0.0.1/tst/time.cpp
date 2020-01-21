#include <chrono>

unsigned long millis()
{
  using namespace std::chrono;
  static auto start = system_clock::now();
  auto now = time_point_cast<milliseconds>(system_clock::now());
  auto difference = duration_cast<milliseconds>(now - start);
  return difference.count();
}