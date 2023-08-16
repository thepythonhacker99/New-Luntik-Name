#include "Game.h"

#include "../Renderer/Window.h"
#include "Client/Client.h"
#include "Entities/PlayerInfo.h"
#include "Packets.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Vector2.hpp"
#include "Server/Server.h"
#include "Textures.h"

#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>
#include <unordered_map>

namespace Luntik {
Game::Game(sf::IpAddress ip) : m_Ip(ip) {
  m_Window = new Renderer::Window("Luntik", 540);
}

void Game::run() {
  Networking::registerPacket<Packets::C2S_CHUNK_PACKET, Utils::Pos>();
  Networking::registerPacket<Packets::S2C_CHUNK_PACKET, GameObjects::Chunk>();

  Networking::registerPacket<Packets::S2C_PLAYER_ID_PACKET, ID_t>();

  Networking::registerPacket<Packets::S2C_PLAYERS_INFO_PACKET,
                             std::unordered_map<ID_t, Entities::PlayerInfo>>();

  Networking::registerPacket<Packets::S2C_PLAYER_DISCONNECTED_PACKET, ID_t>();

  Networking::registerPacket<Packets::S2C_PLAYERS_POSITION_PACKET,
                             std::unordered_map<ID_t, sf::Vector2f>>();

  Networking::registerPacket<Packets::C2S_POSITION_PACKET, sf::Vector2f>();

  Textures::loadTextures();

  constexpr int port = 6969;

  SPDLOG_INFO("Running game on address: {} and port {}", m_Ip.toString(), port);

  Server::Server *server = new Server::Server(m_Ip, port);

  Client::Client *client = new Client::Client(m_Ip, port, m_Window);

  std::thread serverThread([&server]() {
    server->start();
    server->run();
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::thread clientThread([&client]() {
    client->start();
    client->run();
  });

  if (clientThread.joinable())
    clientThread.join();

  delete client;

  if (server->isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    server->stop();
  }

  if (serverThread.joinable())
    serverThread.join();

  delete server;

  SPDLOG_INFO("Ended running");
}
} // namespace Luntik
