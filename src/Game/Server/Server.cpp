#include "Server.h"

#include "../../Utils/Pos.h"
#include "../../Utils/Timers.h"
#include "../Entities/PlayerInfo.h"
#include "../GameObjects/Terrain.h"
#include "../Packets.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Cursor.hpp"
#include "random/random.h"
#include "spdlog/spdlog.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
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

  m_SocketServer.setClientConnectedCallback([this](ID_t id) {
    addPlayer(id);

    SPDLOG_INFO("Client connected with network handle: {}", id);
  });

  m_SocketServer.setClientDisconnectedCallback([this](ID_t id) {
    SPDLOG_INFO("Client with id {} disconnected!", id);
    removePlayer(id);
    // if (m_SocketServer.clientsCount() == 0) {
    //   stop();
    // }
  });

  m_SocketServer.addReceiveCallback(Packets::C2S_POSITION_PACKET,
                                    std::function<void(ID_t, sf::Vector2f)>(
                                        [this](ID_t id, sf::Vector2f pos) {
                                          if (m_GameState.hasPlayer(id)) {
                                            m_GameState.players.at(id).pos =
                                                pos;
                                          } else {
                                            // SPDLOG_INFO("Received pos from
                                            // non-registered player id {}",
                                            // id);
                                          }
                                        }));

  m_SocketServer.addReceiveCallback(
      Packets::C2S_CHUNK_PACKET,
      std::function<void(ID_t, Utils::Pos)>(
          [this](ID_t senderId, Utils::Pos pos) {
            // SPDLOG_INFO("Received chunk request with pos {} {} from id {}",
            //             pos.x, pos.y, senderId);
            //             m_SocketServer.send(

            GameObjects::Chunk *chunk =
                m_GameState.world.getTerrain()->getChunkOrGenerate(pos);

            m_SocketServer.send(
                senderId,
                Networking::createPacket<Packets::S2C_CHUNK_PACKET>(*chunk));
          }));

  m_SocketServer.start();
  if (!m_SocketServer.isListenThreadRunning()) {
    SPDLOG_INFO("Failed to start SocketServer!");
    return;
  }

  m_Running = true;
}

void Server::addPlayer(ID_t handleId) {
  Entities::PlayerInfo info(handleId);
  info.pos = sf::Vector2f(150, 150);

  m_GameState.players.emplace(handleId, info);

  m_SocketServer.send(
      handleId,
      Networking::createPacket<Packets::S2C_PLAYER_ID_PACKET>(handleId));

  std::unordered_map<ID_t, Entities::PlayerInfo> newPlayer;
  newPlayer.emplace(handleId, info);

  m_SocketServer.sendAll(
      Networking::createPacket<Packets::S2C_PLAYERS_INFO_PACKET>(newPlayer),
      handleId);

  m_SocketServer.send(
      handleId, Networking::createPacket<Packets::S2C_PLAYERS_INFO_PACKET>(
                    m_GameState.players));
}

void Server::removePlayer(ID_t handleId) {
  m_GameState.players.erase(handleId);

  m_SocketServer.sendAll(
      Networking::createPacket<Packets::S2C_PLAYER_DISCONNECTED_PACKET>(
          handleId));
}

void Server::stop() {
  if (!isRunning()) {
    SPDLOG_WARN("Server isn't on");
    return;
  }

  m_GameState.world.getTerrain()->getTerrain()->clear();
  m_GameState.players.clear();

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

  m_SocketServer.sendAll(
      Networking::createPacket<Packets::S2C_PLAYERS_POSITION_PACKET>(
          m_GameState.getPlayersPosMap()));
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

  Utils::Timers::BlockingTimer<Settings::SERVER_TPS> serverTimer;

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
