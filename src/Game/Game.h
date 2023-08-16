#pragma once

#include "../Renderer/Window.h"
#include "SFML/Network/IpAddress.hpp"

namespace Luntik {
class Game : Utils::NonCopyable {
public:
  Game(sf::IpAddress ip);

  void run();

private:
  sf::IpAddress m_Ip;

  Renderer::Window *m_Window;
};
} // namespace Luntik
