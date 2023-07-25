#pragma once

namespace Luntik::Utils {
class NonCopyable {
public:
  NonCopyable() = default;

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
};
} // namespace Luntik::Utils
