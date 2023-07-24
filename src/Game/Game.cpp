#include "Game.h"

#include "../Renderer/Window.h"
#include "Client/Client.h"
#include "Packets.h"
#include "SFML/Config.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "Server/Server.h"

#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>

namespace Luntik {
Game::Game() { m_Window = new Renderer::Window("Luntik", 360); }

void Game::run() {
  Networking::registerPacket<Packets::C2S_CHUNK_PACKET, Utils::Pos>();
  Networking::registerPacket<Packets::S2C_CHUNK_PACKET, GameObjects::Chunk>();

  Server::Server *server = new Server::Server(
      sf::IpAddress::getLocalAddress().value_or(sf::IpAddress::LocalHost),
      6969);
  Client::Client *client = new Client::Client(
      sf::IpAddress::getLocalAddress().value_or(sf::IpAddress::LocalHost), 6969,
      m_Window);

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

  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  // server->end();

  if (serverThread.joinable())
    serverThread.join();

  delete server;
}
} // namespace Luntik
