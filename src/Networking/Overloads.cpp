#include "Overloads.h"

#include <cstdint>
#include <iostream>

sf::Packet &operator<<(sf::Packet &packet, const Luntik::Utils::Pos &pos) {
  return packet << pos.x << pos.y;
}

sf::Packet &operator>>(sf::Packet &packet, Luntik::Utils::Pos &pos) {
  return packet >> pos.x >> pos.y;
}

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::BlockType &type) {
  return packet << static_cast<uint8_t>(type);
}

sf::Packet &operator>>(sf::Packet &packet,
                       Luntik::GameObjects::BlockType &type) {
  uint8_t typeUint8;
  packet >> typeUint8;
  type = static_cast<Luntik::GameObjects::BlockType>(typeUint8);
  return packet;
}

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::Chunk &chunk) {
  packet << chunk.pos;

  for (uint16_t i = 0; i < Luntik::Settings::CHUNK_SIZE_SQUARED; i++) {
    packet << chunk.blocks[i].type;
  }

  return packet;
}

sf::Packet &operator>>(sf::Packet &packet, Luntik::GameObjects::Chunk &chunk) {
  packet >> chunk.pos;

  for (uint16_t i = 0; i < Luntik::Settings::CHUNK_SIZE_SQUARED; i++) {
    packet >> chunk.blocks[i].type;
    chunk.blocks[i].pos =
        Luntik::Utils::Pos(uint16_t(i % Luntik::Settings::CHUNK_SIZE),
                           uint16_t(i / Luntik::Settings::CHUNK_SIZE));
  }

  return packet;
}

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::Entities::PlayerInfo &player) {
  return packet << player.socketHandle << player.pos;
}

sf::Packet &operator>>(sf::Packet &packet,
                       Luntik::Entities::PlayerInfo &player) {
  return packet >> player.socketHandle >> player.pos;
}
