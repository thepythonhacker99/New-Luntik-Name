#pragma once

#include "../settings.h"

namespace Luntik::Components {
struct UUIDComponent {
  ID_t uuid = ID_t_MAX;

  UUIDComponent() = default;
  UUIDComponent(const UUIDComponent &) = default;
  UUIDComponent(UUIDComponent &) = default;

  UUIDComponent(ID_t id) : uuid(id) {}

  operator const ID_t &() const { return uuid; }
  operator ID_t &() { return uuid; }
};
} // namespace Luntik::Components
