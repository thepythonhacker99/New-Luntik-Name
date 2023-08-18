#include "ClientPlayerEntity.h"

#include "../../Textures.h"
#include "ClientHumanEntity.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "spdlog/spdlog.h"

namespace Luntik::Entities {
ClientPlayerEntity::ClientPlayerEntity(ID_t id, PlayerInfo *player)
    : ClientHumanEntity(id), m_Player(player) {}

void ClientPlayerEntity::tick(float deltaTime) {
  constexpr float speed = 10 * Settings::BLOCK_SIZE;

  // SPDLOG_INFO("POS: {} {}, ID: {}", m_Pos.x, m_Pos.y,
  // m_Player->socketHandle);

  sf::Vector2f moveVec;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    moveVec.x += -1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    moveVec.x += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
    moveVec.y += -1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
    moveVec.y += 1;
  }

  if (moveVec.x != 0 || moveVec.y != 0) {
    sf::Vector2f newPos = getPos() + moveVec.normalized() * speed * deltaTime;

    setPos(newPos);
    m_Player->pos = newPos;
  }

  // SPDLOG_INFO("{} {}", m_Pos.x, m_Pos.y);
}
} // namespace Luntik::Entities
