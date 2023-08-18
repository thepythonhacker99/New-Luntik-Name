#include "SFML/System/Vector2.hpp"
#include "Systems.h"

#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "spdlog/spdlog.h"

namespace Luntik::Systems {
void ClientRenderSystem(entt::registry &registry, Renderer::Window *window) {
  registry.view<Components::RenderComponent, Components::PositionComponent>()
      .each([&](auto entity, Components::RenderComponent &renderComponent,
                Components::PositionComponent positionComponent) {
        if (!renderComponent.texture) {
          SPDLOG_ERROR("Texture of render component is null");
          return;
        }

        sf::Sprite sprite(*renderComponent.texture);
        sprite.setPosition(positionComponent.pos);
        sprite.setOrigin(
            sf::Vector2f(renderComponent.texture->getSize().x / 2.f,
                         renderComponent.texture->getSize().y / 2.f));

        window->getInternalWindow()->draw(sprite);
      });
}
} // namespace Luntik::Systems
