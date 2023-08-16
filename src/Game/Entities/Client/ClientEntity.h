#pragma once

#include "../../../Renderer/Window.h"
#include "../Entity.h"

namespace Luntik::Entities {
class ClientEntity : public Entity {
public:
  ClientEntity(ID_t id = ID_t_MAX);

  virtual void render(Renderer::Window *window){};

private:
};
} // namespace Luntik::Entities
