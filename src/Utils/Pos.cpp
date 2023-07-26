#include "Pos.h"

std::ostream &operator<<(std::ostream &out, const Luntik::Utils::Pos &pos) {
  out << pos.x << " " << pos.y;
  return out;
}
