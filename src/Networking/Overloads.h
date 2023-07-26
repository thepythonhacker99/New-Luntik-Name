#pragma once

#include "../Game/GameObjects/Terrain.h"
#include "../Utils/Pos.h"
#include "SFML/Network.hpp"

sf::Packet &operator<<(sf::Packet &packet, const Luntik::Utils::Pos &pos);
sf::Packet &operator>>(sf::Packet &packet, Luntik::Utils::Pos &pos);

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::BlockType &type);

sf::Packet &operator>>(sf::Packet &packet,
                       Luntik::GameObjects::BlockType &type);

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::Chunk &chunk);

sf::Packet &operator>>(sf::Packet &packet, Luntik::GameObjects::Chunk &chunk);
