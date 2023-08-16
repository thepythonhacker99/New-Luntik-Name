#include "Entity.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Entities {
Entity::Entity(ID_t id) : m_Id(id) {}

ID_t Entity::getId() const { return m_Id; }

sf::Vector2f Entity::getPos() const { return m_Pos; }
} // namespace Luntik::Entities
