#pragma once

#include "Overloads.h"
#include "SFML/Network/Packet.hpp"

#include <algorithm>
#include <any>
#include <bits/utility.h>
#include <cstddef>
#include <exception>
#include <functional>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace Luntik::Networking {
template <typename T> T packetReader(sf::Packet &packet) {
  // SPDLOG_INFO("READING FROM PACKET {}", Utils::type_name<T>());
  T instance;
  packet >> instance;
  return instance;
}

template <typename T> void packetWriter(sf::Packet &packet, const T &value) {
  // SPDLOG_INFO("WRITING TO PACKET {}", Utils::type_name<T>());
  packet << value;
}

inline std::unordered_map<ID_t, std::any> registeredPackets;

bool isPacketRegistered(ID_t id);

template <typename T> T convertArgument(std::any arg) {
  return std::any_cast<T>(arg);
}

template <typename... args_t> bool isPacketArgsValid(ID_t id) {
  auto it = registeredPackets.find(id);
  if (it == registeredPackets.end()) {
    return false;
  }

  try {
    // std::any_cast<std::tuple<std::decay_t<args_t>...>>(it->second);
    std::any_cast<std::tuple<args_t...>>(it->second);
  } catch (const std::bad_any_cast &) {
    return false;
  }

  return true;
}

template <ID_t id, typename... args_t> void registerPacket() {
  if (isPacketRegistered(id)) {
    throw std::runtime_error(
        "Error: 'registerPacket' called with duplicate id. id: " +
        std::to_string(id));
  }

  registeredPackets.emplace(id, std::make_any<std::tuple<args_t...>>());
}

template <ID_t id, typename... args_t> sf::Packet createPacket(args_t... args) {
  if (!isPacketRegistered(id)) {
    throw std::runtime_error("Error: called 'createPacket' but the packet is "
                             "not registered. id: " +
                             std::to_string(id));
  }

  if (!isPacketArgsValid<args_t...>(id)) {
    throw std::runtime_error(
        "Error: called 'createPacket' but the args are invalid. id: " +
        std::to_string(id));
  }

  sf::Packet packet;
  packetWriter(packet, id);

  int a;
  ((packetWriter(packet, std::forward<args_t>(args)), a) = ...);

  return std::move(packet);
}

ID_t getPacketType(sf::Packet &packet);
} // namespace Luntik::Networking
