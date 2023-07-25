#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>

namespace Luntik::Utils::Timers {
template <std::size_t FPS> class BlockingTimer {
public:
  BlockingTimer()
      : m_FrameTime{1}, m_TimePoint{std::chrono::steady_clock::now()} {}

  void sleep() {
    m_TimePoint += m_FrameTime;
    std::this_thread::sleep_until(m_TimePoint);
  };

private:
  std::chrono::duration<double, std::ratio<1, FPS>> m_FrameTime;

  // the time point we'll add to in every loop
  std::chrono::time_point<std::chrono::steady_clock, decltype(m_FrameTime)>
      m_TimePoint;
};

template <size_t FPS> class NonBlockingTimer {
public:
  NonBlockingTimer() : m_TimeStep(double(1) / double(FPS)) {}

  bool timeReached(double delta) {
    m_Count += delta;

    if (m_Count >= m_TimeStep) {
      m_Count -= m_TimeStep;
      return true;
    }

    return false;
  }

private:
  double m_TimeStep;
  double m_Count = 0.0;
};
} // namespace Luntik::Utils::Timers
