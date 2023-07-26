#pragma once

#include "../Utils/NonCopyable.h"
#include "Common.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "spdlog/spdlog.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace Luntik::Networking {
using ClientReceiveInternalCallback = std::function<void(sf::Packet)>;

class SocketClient : Utils::NonCopyable {
public:
  SocketClient(sf::IpAddress ip, uint16_t port);
  ~SocketClient();

  bool isRunning();

  void start();
  void stop();

  void send(sf::Packet packet);

  void handleCallbacks();

  template <typename... args_t>
  void addReceiveCallback(ID_t id, std::function<void(args_t...)> callback) {
    if (!isPacketRegistered(id)) {
      SPDLOG_ERROR("Packet with id {} hasn't beed registered yet", id);
      return;
    }

    if (!isPacketArgsValid<args_t...>(id)) {
      SPDLOG_ERROR("Args of callback function are not valid {}", id);
      return;
    }

    if (m_Callbacks.find(id) != m_Callbacks.end()) {
      SPDLOG_WARN("Warning: overriding callback with id {}", id);
    }

    m_Callbacks[id] = [callback](sf::Packet packet) {
      callback(packetReader<args_t>(packet)...);
    };
  }

private:
  void clientThread();

  sf::TcpSocket m_Socket;

  std::atomic<bool> m_ClientThreadRunning = false;
  std::thread m_ClientThread;

  std::unordered_map<ID_t, ClientReceiveInternalCallback> m_Callbacks;

  std::mutex m_ReceivedPacketsMutex;
  std::vector<sf::Packet> m_ReceivedPackets;

  sf::IpAddress m_Ip;
  uint16_t m_Port;
};
} // namespace Luntik::Networking
