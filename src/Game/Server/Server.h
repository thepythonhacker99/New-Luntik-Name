#pragma once

#include "../../Networking/SocketServer.h"
#include "../../Utils/NonCopyable.h"
#include "../../Utils/Pos.h"
#include "../GameObjects/Terrain.h"
#include "../GameState.h"
#include "SFML/Network/IpAddress.hpp"
#include <atomic>
#include <cstdint>

namespace Luntik::Server {
class Server : Utils::NonCopyable {
public:
  Server(sf::IpAddress ip, uint16_t port);
  ~Server();

  bool isRunning() const;

  void start();
  void stop();

  void tick(float deltaTime);
  void run();

private:
  void addPlayer(ID_t handleId);
  void removePlayer(ID_t handleId);

  Networking::SocketServer m_SocketServer;

  GameState m_GameState;

  sf::IpAddress m_Ip;
  uint16_t m_Port;

  std::atomic<bool> m_Running = false;
};
} // namespace Luntik::Server
