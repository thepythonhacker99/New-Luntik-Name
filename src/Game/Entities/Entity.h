#pragma once

#include "../settings.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Entities {
class Entity {
public:
  Entity(ID_t id = ID_t_MAX);

  virtual void tick(float deltaTime){};

  ID_t getId() const;
  sf::Vector2f getPos() const;

protected:
  ID_t m_Id;

  sf::Vector2f m_Pos;
};
} // namespace Luntik::Entities
