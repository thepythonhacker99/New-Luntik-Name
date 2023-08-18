#include "ClientHumanEntity.h"

#include "../../Textures.h"

namespace Luntik::Entities {
ClientHumanEntity::ClientHumanEntity(ID_t id) : ClientEntity(id) {}

void ClientHumanEntity::render(Renderer::Window *window) {
  sf::Sprite playerSprite(Textures::s_PlayerTexture);
  playerSprite.setOrigin(
      sf::Vector2f(Settings::BLOCK_SIZE_HALF, Settings::BLOCK_SIZE_HALF));
  playerSprite.setPosition(getPos());

  window->getInternalWindow()->draw(playerSprite);
}
} // namespace Luntik::Entities
