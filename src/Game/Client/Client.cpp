#include "Client.h"

#include "../../Utils/Pos.h"
#include "../../Utils/Timers.h"
#include "../Packets.h"
#include "SFML/Network/IpAddress.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace Luntik::Client {
Client::Client(sf::IpAddress ip, uint16_t port, Renderer::Window *window)
    : m_Window(window), m_Ip(ip), m_Port(port),
      m_TerrainManager(&m_GameState.terrain, m_Window)
//, m_SocketClient(ip, port)
{}

Client::~Client() {
  if (isRunning())
    stop();
}

bool Client::isRunning() { return m_Running; }

void Client::start() {
  if (isRunning()) {
    SPDLOG_WARN("Client is already on");
    return;
  }

  m_Running = true;

  // m_SocketClient.addReceiveCallback(
  //     Packets::S2C_CHUNK_PACKET,
  //     std::function<void(Networking::ID_t, GameObjects::Chunk)>(
  //         [this](Networking::ID_t id, GameObjects::Chunk chunk) {
  //           SPDLOG_INFO("Received chunk with pos {} {}", chunk.pos.x,
  //                       chunk.pos.y);
  //           m_GameState.terrain.getTerrain()->operator[](chunk.pos) = chunk;
  //         }));

  // m_SocketClient.start();

  // m_SocketClient.send(
  //     Networking::createPacket<Packets::C2S_CHUNK_PACKET>(Utils::Pos{0, 0}));
}

void Client::stop() {
  if (!isRunning()) {
    SPDLOG_WARN("Client isn't on");
    return;
  }

  m_Running = false;
  // m_SocketClient.stop();
}

void Client::tick(float deltaTime) {
  if (!isRunning()) {
    SPDLOG_WARN("Client isn't on");
    return;
  }

  m_Window->render(deltaTime);
  if (!m_Window->isOpen()) {
    stop();
    return;
  }

  m_TerrainManager.tick(deltaTime);
  m_GameState.terrain.render(m_Window);

  m_Window->display();
}

void Client::run() {
  if (!isRunning()) {
    SPDLOG_WARN("Client isn't on");
    return;
  }

  sf::Clock m_Clock;

  float deltaTime = 0.f;
  int fps = 0;
  float timeForFps = 0.f;

  while (isRunning()) {
    deltaTime = m_Clock.restart().asSeconds();

    timeForFps += deltaTime;
    fps++;
    if (timeForFps >= 1.f) {
      SPDLOG_INFO("FPS: {0}", fps);
      timeForFps -= 1.f;
      fps = 0;
    }

    tick(deltaTime);
  }

  SPDLOG_INFO("Ended running");
}
} // namespace Luntik::Client
