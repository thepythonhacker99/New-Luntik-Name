#include "Client.h"

#include "../../Networking/Common.h"
#include "../../Utils/Math.h"
#include "../../Utils/Pos.h"
#include "../../Utils/Timers.h"
#include "../Entities/Client/ClientPlayerEntity.h"
#include "../Packets.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Luntik::Client {
Client::Client(sf::IpAddress ip, uint16_t port, Renderer::Window *window)
    : m_Window(window), m_Ip(ip), m_Port(port),
      m_TerrainManager(&m_GameState.terrain, &m_SocketClient, m_Window),
      m_SocketClient(ip, port) {}

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

  m_SocketClient.setDisconnectionCallback([this]() {
    SPDLOG_INFO("You got disconnected from the server!");
    stop();
  });

  m_SocketClient.addReceiveCallback(
      Packets::S2C_CHUNK_PACKET,
      std::function<void(GameObjects::Chunk)>([this](GameObjects::Chunk chunk) {
        // SPDLOG_INFO("Received chunk with pos {} {}", chunk.pos.x,
        // chunk.pos.y);
        m_TerrainManager.onChunkReceive(chunk);
      }));

  m_SocketClient.addReceiveCallback(Packets::S2C_PLAYER_ID_PACKET,
                                    std::function<void(ID_t)>([this](ID_t id) {
                                      m_PlayerId = id;
                                      SPDLOG_INFO("Got player id: {}", id);
                                    }));

  m_SocketClient.addReceiveCallback(
      Packets::S2C_PLAYERS_INFO_PACKET,
      std::function<void(std::unordered_map<ID_t, Entities::PlayerInfo>)>(
          [this](std::unordered_map<ID_t, Entities::PlayerInfo> players) {
            for (auto &[id, info] : players) {
              bool playerExists = m_GameState.hasPlayer(id);

              Entities::PlayerInfo &playerInfoClient = m_GameState.players[id];
              playerInfoClient = info;

              if (!playerExists) {
                Entities::ClientPlayerEntity *playerEntity =
                    m_EntityManager.addEntity(id,
                                              new Entities::ClientPlayerEntity(
                                                  id, &playerInfoClient));
                SPDLOG_INFO("JESSE");
              }
            }
          }));

  m_SocketClient.addReceiveCallback(
      Packets::S2C_PLAYERS_POSITION_PACKET,
      std::function<void(std::unordered_map<ID_t, sf::Vector2f>)>(
          [this](std::unordered_map<ID_t, sf::Vector2f> players) {
            for (auto &[id, pos] : players) {
              if (!m_GameState.hasPlayer(id)) {
                SPDLOG_ERROR("Received pos for player with id {}, but the "
                             "player does not exist!",
                             id);
                continue;
              }

              if (id != m_PlayerId ||
                  Utils::distanceSquared(pos, m_GameState.players.at(id).pos) >
                      Settings::MAX_POS_DIFFERENCE) {
                // SPDLOG_INFO("SETTINGS POS FOR {}", id);
                m_GameState.players.at(id).pos = pos;
              }
            }
          }));

  m_SocketClient.addReceiveCallback(
      Packets::S2C_PLAYER_DISCONNECTED_PACKET,
      std::function<void(ID_t)>([this](ID_t id) {
        m_EntityManager.removeEntity(id);
        m_GameState.players.erase(id);

        SPDLOG_INFO("Player with id {} has disconnected!", id);
      }));

  m_SocketClient.start();
  if (!m_SocketClient.isRunning()) {
    SPDLOG_ERROR("Failed to start socket client!");
    return;
  }

  m_Running = true;
}

void Client::stop() {
  if (!isRunning()) {
    SPDLOG_WARN("Client isn't on");
    return;
  }

  m_Running = false;
  m_SocketClient.stop();
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

  m_SocketClient.handleCallbacks();

  m_TerrainManager.tick(deltaTime);
  m_TerrainManager.render(m_Window);

  m_EntityManager.tick(deltaTime);

  m_EntityManager.render(m_Window);

  if (m_GameState.hasPlayer(m_PlayerId)) {
    Entities::PlayerInfo &info = m_GameState.players.at(m_PlayerId);
    m_Window->getCamera()->setGoal(info.pos);

    m_SocketClient.send(
        Networking::createPacket<Packets::C2S_POSITION_PACKET>(info.pos));
  }

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
