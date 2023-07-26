#include "Common.h"

#include "SFML/Network/Packet.hpp"

namespace Luntik::Networking {
bool isPacketRegistered(ID_t id) {
  return registeredPackets.find(id) != registeredPackets.end();
}

ID_t getPacketType(sf::Packet &packet) {
  ID_t id;
  packet >> id;
  return id;
}
} // namespace Luntik::Networking
