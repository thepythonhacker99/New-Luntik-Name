#include "Pos.h"
#include "SFML/Network/Packet.hpp"

std::ostream &operator<<(std::ostream &out, const Luntik::Utils::Pos &pos) {
  out << pos.x << " " << pos.y;
  return out;
}

sf::Packet &operator<<(sf::Packet &packet, const Luntik::Utils::Pos &pos) {
  return packet << pos.x << pos.y;
}

sf::Packet &operator>>(sf::Packet &packet, Luntik::Utils::Pos &pos) {
  return packet >> pos.x >> pos.y;
}
