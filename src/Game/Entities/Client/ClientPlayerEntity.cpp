#include "ClientPlayerEntity.h"

#include "../../Textures.h"
#include "../../settings.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "spdlog/spdlog.h"

namespace Luntik::Entities {
ClientPlayerEntity::ClientPlayerEntity(ID_t id, PlayerInfo *player)
    : ClientEntity(id), m_Player(player) {}

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
    m_Player->pos += moveVec.normalized() * speed * deltaTime;
  }

  m_Pos = m_Player->pos;

  // SPDLOG_INFO("{} {}", m_Pos.x, m_Pos.y);
}

void ClientPlayerEntity::render(Renderer::Window *window) {
  sf::Sprite playerSprite(Textures::s_PlayerTexture);
  playerSprite.setOrigin(
      sf::Vector2f(Settings::BLOCK_SIZE_HALF, Settings::BLOCK_SIZE_HALF));
  playerSprite.setPosition(m_Pos);

  window->getInternalWindow()->draw(playerSprite);
}
} // namespace Luntik::Entities
