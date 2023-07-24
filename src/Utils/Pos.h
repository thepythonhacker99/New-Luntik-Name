#pragma once

#include "SFML/Network/Packet.hpp"
#include <cstdint>
#include <functional>
#include <ostream>
#include <utility>

namespace Luntik::Utils {
struct Pos {
  int x, y;

  Pos() : x(0), y(0) {}
  Pos(int x, int y) : x(x), y(y) {}

  bool operator==(Pos other) const { return other.x == x && other.y == y; }

  Pos operator+(int other) const { return Pos{x + other, y + other}; }
  Pos operator-(int other) const { return Pos{x - other, y - other}; }
  Pos operator*(int other) const { return Pos{x * other, y * other}; }
  Pos operator/(int other) const { return Pos{x / other, y / other}; }

  bool operator==(const Pos &other) { return x == other.x && y == other.y; }
  bool operator!=(const Pos &other) { return !(*this == other); }
};
} // namespace Luntik::Utils

template <> struct std::hash<Luntik::Utils::Pos> {
  std::size_t operator()(const Luntik::Utils::Pos &pos) const {
    int a = (pos.x >= 0 ? 2 * pos.x : (-2 * pos.x) - 1);
    int b = (pos.y >= 0 ? 2 * pos.y : (-2 * pos.y) - 1);

    return (a >= b ? (a * a) + a + b : (b * b) + a);
  }
};

inline std::ostream &operator<<(std::ostream &out,
                                const Luntik::Utils::Pos &pos) {
  out << pos.x << " " << pos.y;
  return out;
}

inline sf::Packet &operator<<(sf::Packet &packet,
                              const Luntik::Utils::Pos &pos) {
  return packet << pos.x << pos.y;
}

inline sf::Packet &operator>>(sf::Packet &packet, Luntik::Utils::Pos &pos) {
  return packet >> pos.x >> pos.y;
}
