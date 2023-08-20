#pragma once

#include "../../Networking/SocketClient.h"
#include "../../Renderer/Window.h"
#include "../../Utils/Timers.h"
#include "../GameObjects/TerrainManager.h"
#include "../GameObjects/World.h"
#include "../GameState.h"
#include "SFML/Network/IpAddress.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"

#include <atomic>
#include <cstdint>
#include <limits>

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
  ID_t m_PlayerId = ID_t_MAX;

  Utils::Timers::NonBlockingTimer<20> m_SendPosTimer;

  sf::IpAddress m_Ip;
  uint16_t m_Port;

  Networking::SocketClient m_SocketClient;

  GameState m_GameState;
  Renderer::Window *m_Window;

  GameObjects::World m_World;

  GameObjects::TerrainManager m_TerrainManager;

  std::atomic<bool> m_Running = false;
};
} // namespace Luntik::Client
