#include "Server.h"

#include "../../Utils/Pos.h"
#include "../../Utils/Timers.h"
#include "../GameObjects/Terrain.h"
#include "../Packets.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Clock.hpp"
#include "spdlog/spdlog.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

namespace Luntik::Server {
Server::Server(sf::IpAddress ip, uint16_t port)
    : m_Ip(ip), m_Port(port), m_SocketServer(ip, port) {}

Server::~Server() {
  if (isRunning())
    stop();
}

bool Server::isRunning() const { return m_Running; }

void Server::start() {
  if (isRunning()) {
    SPDLOG_WARN("Serevr is already on");
    return;
  }

  m_SocketServer.setClientConnectedCallback([](Networking::ID_t id) {
    SPDLOG_INFO("Client connected with id: {}", id);
  });

  m_SocketServer.setClientDisconnectedCallback([this](Networking::ID_t id) {
    SPDLOG_INFO("Client with id {} disconnected!", id);
    if (m_SocketServer.clientsCount() == 0) {
      // stop();
    }
  });

  m_SocketServer.addReceiveCallback(
      Packets::C2S_CHUNK_PACKET,
      std::function<void(Networking::ID_t, Utils::Pos)>(
          [this](Networking::ID_t senderId, Utils::Pos pos) {
            SPDLOG_INFO("Received chunk request with pos {} {} from id {}",
                        pos.x, pos.y, senderId);
            m_SocketServer.send(
                senderId, Networking::createPacket<Packets::S2C_CHUNK_PACKET>(
                              GameObjects::Chunk(pos)));
          }));

  m_SocketServer.start();
  if (!m_SocketServer.isListenThreadRunning()) {
    SPDLOG_INFO("Failed to start SocketServer!");
    return;
  }

  m_Running = true;
}

void Server::stop() {
  if (!isRunning()) {
    SPDLOG_WARN("Server isn't on");
    return;
  }

  m_SocketServer.stop();
  m_Running = false;

  SPDLOG_INFO("Stopped");
}

void Server::tick(float deltaTime) {
  if (!isRunning()) {
    SPDLOG_WARN("Server isn't on");
    return;
  }

  m_SocketServer.handleCallbacks();
}

void Server::run() {
  if (!isRunning()) {
    SPDLOG_WARN("Server isn't on");
    return;
  }

  sf::Clock clock;
  float deltaTime = 0.f;
  int fps = 0;
  float timeForFps = 0.f;

  Utils::Timers::BlockingTimer<20> serverTimer;

  m_Running = true;
  while (isRunning()) {
    deltaTime = clock.restart().asSeconds();

    timeForFps += deltaTime;
    fps++;
    if (timeForFps >= 1.f) {
      SPDLOG_INFO("FPS: {0}", fps);
      timeForFps -= 1.f;
      fps = 0;
    }

    tick(deltaTime);

    serverTimer.sleep();
  }

  SPDLOG_INFO("Ended running");
}
} // namespace Luntik::Server
