#include "Client.h"

#include "../../Networking/Common.h"
#include "../../Utils/Math.h"
#include "../../Utils/Pos.h"
#include "../../Utils/Timers.h"
#include "../Components/AccelerationComponent.h"
#include "../Components/BoundingBoxComponent.h"
#include "../Components/FrictionComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/KeyboardMovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/PositionInterpolatorComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/VelocityComponent.h"
#include "../GameObjects/Entity.h"
#include "../Packets.h"
#include "../Systems/Systems.h"
#include "../Textures.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "spdlog/spdlog.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Luntik::Client {
Client::Client(sf::IpAddress ip, uint16_t port, Renderer::Window *window)
    : m_Window(window), m_Ip(ip), m_Port(port),
      m_TerrainManager(m_GameState.world.getTerrain(), &m_SocketClient,
                       m_Window),
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
                if (id == m_PlayerId) {
                  GameObjects::Entity playerEntity = m_GameState.world.create();
                  playerEntity.add<Components::UUIDComponent>(id);

                  playerEntity.add<Components::KeyboardMovementComponent>();
                  playerEntity.add<Components::PositionComponent>(info.pos);
                  playerEntity.add<Components::VelocityComponent>();
                  playerEntity.add<Components::FrictionComponent>();
                  playerEntity.add<Components::AccelerationComponent>();
                  // playerEntity.add<Components::GravityComponent>();
                  playerEntity.add<Components::BoundingBoxComponent>(
                      sf::Vector2f(14, 14));

                  auto &renderComponent =
                      playerEntity.add<Components::RenderComponent>();
                  renderComponent.texture = &Textures::s_PlayerTexture;
                } else {
                  GameObjects::Entity playerEntity = m_GameState.world.create();
                  playerEntity.add<Components::UUIDComponent>(id);

                  playerEntity.add<Components::PositionComponent>(info.pos);

                  auto &positionInterpolatorComponent =
                      playerEntity
                          .add<Components::PositionInterpolatorComponent>();

                  auto &renderComponent =
                      playerEntity.add<Components::RenderComponent>();
                  renderComponent.texture = &Textures::s_PlayerTexture;
                }
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

              GameObjects::Entity entity =
                  m_GameState.world.getEntityByUUID(id);

              if (!entity.isValid()) {
                SPDLOG_ERROR(
                    "Received pos for player with id {} but the player "
                    "entity is not registered!",
                    id);
                continue;
              }

              if (id == m_PlayerId) {
                if (Utils::distanceSquared(pos,
                                           m_GameState.players.at(id).pos) >
                    Settings::MAX_POS_DIFFERENCE_SQ) {
                  auto &positionComponent =
                      entity.get<Components::PositionComponent>();
                  positionComponent = pos;
                }
              } else {
                auto &positionInterpolatorComponent =
                    entity.get<Components::PositionInterpolatorComponent>();
                positionInterpolatorComponent.setGoal(pos);
              }
            }
          }));

  m_SocketClient.addReceiveCallback(
      Packets::S2C_PLAYER_DISCONNECTED_PACKET,
      std::function<void(ID_t)>([this](ID_t id) {
        m_GameState.players.erase(id);

        GameObjects::Entity player = m_GameState.world.getEntityByUUID(id);

        if (!player.isValid()) {
          SPDLOG_INFO(
              "Tried to remove entity but its not present in m_Entites");
        } else {
          m_GameState.world.destroy(player);
        }

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

  Systems::ClientKeyboardMovementSystem(m_GameState.world, deltaTime);
  Systems::MovementSystem(m_GameState.world, m_GameState.world.getTerrain(),
                          deltaTime);
  Systems::GravitySystem(m_GameState.world, deltaTime);

  Systems::PositionInterpolatorSystem(m_GameState.world, deltaTime);

  Systems::ClientRenderSystem(m_GameState.world, m_Window);

  if (m_GameState.hasPlayer(m_PlayerId)) {
    GameObjects::Entity player = m_GameState.world.getEntityByUUID(m_PlayerId);
    Entities::PlayerInfo &info = m_GameState.players.at(m_PlayerId);
    info.pos = player.get<Components::PositionComponent>().pos;

    m_Window->getCamera()->setGoal(info.pos);

    if (m_SendPosTimer.timeReached(deltaTime)) {
      m_SocketClient.send(
          Networking::createPacket<Packets::C2S_POSITION_PACKET>(info.pos));
    }
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
