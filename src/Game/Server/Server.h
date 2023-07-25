#pragma once

#include "../../Utils/NonCopyable.h"
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
  sf::IpAddress m_Ip;
  uint16_t m_Port;

  std::atomic<bool> m_Running = false;
};
} // namespace Luntik::Server
