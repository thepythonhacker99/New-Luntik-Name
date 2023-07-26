#pragma once

#include "../../Networking/SocketServer.h"
#include "../../Utils/NonCopyable.h"
#include "../../Utils/Pos.h"
#include "../GameObjects/Terrain.h"
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
  Networking::SocketServer m_SocketServer;

  sf::IpAddress m_Ip;
  uint16_t m_Port;

  std::atomic<bool> m_Running = false;
};
} // namespace Luntik::Server
