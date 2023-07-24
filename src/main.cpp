#include <exception>
#include <iostream>

#include "Game/Game.h"
#include "Renderer/Window.h"
#include "SFML/Network/Packet.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"

int main() {
  spdlog::set_pattern("[%H:%M:%S] [%!] [%s:%#] %v");

  Luntik::Game game;

  try {
    game.run();
  } catch (const std::exception &e) {
    SPDLOG_ERROR(e.what());
  }

  return 0;
}
