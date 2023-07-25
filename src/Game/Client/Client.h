#pragma once

#include "../../Renderer/Window.h"
#include "../GameObjects/TerrainManager.h"
#include "../GameState.h"
#include "SFML/Network/IpAddress.hpp"

#include <cstdint>

namespace Luntik::Client {
class Client : Utils::NonCopyable {
public:
  Client(sf::IpAddress ip, uint16_t port, Renderer::Window *Window);
  ~Client();

  void start();
  void stop();

  bool isRunning();

  void tick(float delta);
  void run();

private:
  sf::IpAddress m_Ip;
  uint16_t m_Port;

  GameState m_GameState;
  Renderer::Window *m_Window;

  GameObjects::TerrainManager m_TerrainManager;

  std::atomic<bool> m_Running = false;
};
} // namespace Luntik::Client
