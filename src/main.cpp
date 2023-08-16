#include <exception>
#include <iostream>

#include "Game/Game.h"
#include "Renderer/Window.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
  spdlog::set_pattern("[%H:%M:%S] [%!] [%s:%#] %v");

  sf::IpAddress address =
      sf::IpAddress::getLocalAddress().value_or(sf::IpAddress::LocalHost);

  SPDLOG_INFO("{}", argc);

  if (argc == 2) {
    std::optional<sf::IpAddress> newIp = sf::IpAddress::resolve(argv[1]);

    if (newIp.has_value()) {
      SPDLOG_INFO("Got address from argument: {}", argv[1]);
      address = sf::IpAddress::resolve(argv[1]).value();
    } else {
      SPDLOG_INFO("Unable to resolve ip address from {}", argv[1]);
    }
  }

  Luntik::Game game(address);

  try {
    game.run();
  } catch (const std::exception &e) {
    SPDLOG_ERROR(e.what());
  }

  return 0;
}
