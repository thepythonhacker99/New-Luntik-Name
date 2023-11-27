#pragma once

#include "SFML/System/Vector2.hpp"
#include <cstdint>
#include <functional>
#include <ostream>
#include <utility>

namespace Luntik::Utils {
struct Pos {
  int x, y;

  Pos() : x(0), y(0) {}
  Pos(int x, int y) : x(x), y(y) {}

  Pos(const sf::Vector2f &vec)
      : x(static_cast<int>(vec.x)), y(static_cast<int>(vec.y)) {}

  Pos operator+(const int &other) const { return Pos{x + other, y + other}; }
  Pos operator-(const int &other) const { return Pos{x - other, y - other}; }
  Pos operator*(const int &other) const { return Pos{x * other, y * other}; }
  Pos operator/(const int &other) const { return Pos{x / other, y / other}; }

  Pos operator+(const Pos &other) const {
    return Pos{x + other.x, y + other.y};
  }
  Pos operator-(const Pos &other) const {
    return Pos{x - other.x, y - other.y};
  }
  Pos operator*(const Pos &other) const {
    return Pos{x * other.x, y * other.y};
  }
  Pos operator/(const Pos &other) const {
    return Pos{x / other.x, y / other.y};
  }

  bool operator==(const Pos &other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Pos &other) const { return !(*this == other); }
};
} // namespace Luntik::Utils

template <> struct std::hash<Luntik::Utils::Pos> {
  std::size_t operator()(const Luntik::Utils::Pos &pos) const {
    int a = (pos.x >= 0 ? 2 * pos.x : (-2 * pos.x) - 1);
    int b = (pos.y >= 0 ? 2 * pos.y : (-2 * pos.y) - 1);

    return (a >= b ? (a * a) + a + b : (b * b) + a);
  }
};

std::ostream &operator<<(std::ostream &out, const Luntik::Utils::Pos &pos);
