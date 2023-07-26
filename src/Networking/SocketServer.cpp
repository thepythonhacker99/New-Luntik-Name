#include "SocketServer.h"

#include "Common.h"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/Socket.hpp"
#include "SFML/Network/SocketSelector.hpp"
#include "SFML/Network/TcpListener.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "spdlog/spdlog.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <mutex>
#include <thread>

namespace Luntik::Networking {
SocketServer::SocketServer(sf::IpAddress ip, uint16_t port)
    : m_Port(port), m_Ip(ip) {
  m_ClientConnectedCallback = [](ID_t) {};
  m_ClientDisconnectedCallback = [](ID_t) {};
}

SocketServer::~SocketServer() { stop(); }

void SocketServer::start() {
  m_ListenThread = std::thread(&SocketServer::listenThread, this);

  while (!m_ListenThreadRunning && !m_ListenThreadFailed) {
    sf::sleep(sf::milliseconds(100));
  }
}

void SocketServer::stop() {
  m_ListenThreadRunning = false;
  if (m_ListenThread.joinable()) {
    m_ListenThread.join();
  }

  m_ClientsMutex.lock();
  for (auto &[clientId, clientInfo] : m_Clients) {
    clientInfo.isRunning = false;
    clientInfo.socket->disconnect();
    if (clientInfo.thread.joinable()) {
      clientInfo.thread.join();
    }
  }
  m_Clients.clear();
  m_ClientsMutex.unlock();

  SPDLOG_INFO("Stopped");
}

void SocketServer::listenThread() {
  sf::TcpListener listener;

  if (listener.listen(m_Port, m_Ip) != sf::Socket::Status::Done) {
    m_ListenThreadFailed = true;
    return;
  }

  m_ListenThreadRunning = true;

  listener.setBlocking(false);

  while (m_ListenThreadRunning) {
    sf::TcpSocket *newClient = new sf::TcpSocket;
    sf::Socket::Status status = listener.accept(*newClient);
    if (status > sf::Socket::Status::NotReady) {
      SPDLOG_ERROR("Failed to accept client");
      delete newClient;
      m_ListenThreadRunning = false;
      break;
    } else if (status == sf::Socket::Status::Done) {
      ID_t newClientId = m_CurrentClientId++;

      m_ClientsMutex.lock();

      ClientInfo *newClientInfo = &m_Clients[newClientId];
      newClientInfo->id = newClientId;
      newClientInfo->isRunning = true;
      newClientInfo->socket = newClient;
      newClientInfo->thread =
          std::thread(&SocketServer::clientThread, this, newClientInfo);

      m_ClientsMutex.unlock();
    }

    sf::sleep(sf::milliseconds(100));
  }

  listener.close();
}

void SocketServer::clientThread(ClientInfo *clientInfo) {
  sf::SocketSelector selector;
  selector.add(*clientInfo->socket);

  while (clientInfo->isRunning && clientInfo->socket->getRemotePort() != 0 &&
         isListenThreadRunning()) {
    if (selector.wait(sf::milliseconds(10))) {
      sf::Packet packet;
      if (clientInfo->socket->receive(packet) != sf::Socket::Status::Done) {
        SPDLOG_ERROR("Failed to receive packet!");
        break;
      }

      m_ReceivedPacketsMutex.lock();
      m_ReceivedPackets[clientInfo->id].push_back(packet);
      m_ReceivedPacketsMutex.unlock();
    }
  }

  clientInfo->socket->disconnect();
  delete clientInfo->socket;

  clientInfo->isRunning = false;

  m_DisconnectedClientsMutex.lock();
  m_DisconnectedClients.push_back(clientInfo->id);
  m_DisconnectedClientsMutex.unlock();

  SPDLOG_INFO("Finished client {}", clientInfo->id);
}

bool SocketServer::isListenThreadRunning() { return m_ListenThreadRunning; }

size_t SocketServer::clientsCount() {
  std::lock_guard guard(m_ClientsMutex);
  return m_Clients.size();
}

bool SocketServer::clientOnline(ID_t id) {
  std::lock_guard guard(m_ClientsMutex);
  return m_Clients.find(id) != m_Clients.end();
}

void SocketServer::send(ID_t id, sf::Packet packet) {
  std::lock_guard guard(m_ClientsMutex);
  if (m_Clients.at(id).socket->send(packet) != sf::Socket::Status::Done) {
    SPDLOG_ERROR("Failed to send to client {}", id);
  }
}

void SocketServer::setClientConnectedCallback(
    ClientConnectedCallback callback) {
  m_ClientConnectedCallback = callback;
}

void SocketServer::setClientDisconnectedCallback(
    ClientDisconnectedCallback callback) {
  m_ClientDisconnectedCallback = callback;
}

void SocketServer::handleCallbacks() {
  if (!isListenThreadRunning()) {
    SPDLOG_INFO("Listen thread not running");
    return;
  }

  m_ClientsMutex.lock();
  std::erase_if(m_Clients, [](auto &info) {
    if (info.second.isRunning == false) {
      if (info.second.thread.joinable()) {
        info.second.thread.join();
      }
      return true;
    }
    return false;
  });
  m_ClientsMutex.unlock();

  m_ConnectedClientsMutex.lock();
  for (ID_t id : m_ConnectedClients) {
    m_ClientConnectedCallback(id);
  }
  m_ConnectedClients.clear();
  m_ConnectedClientsMutex.unlock();

  m_DisconnectedClientsMutex.lock();
  for (ID_t id : m_DisconnectedClients) {
    m_ClientDisconnectedCallback(id);
  }
  m_DisconnectedClients.clear();
  m_DisconnectedClientsMutex.unlock();

  m_ReceivedPacketsMutex.lock();
  for (auto &[senderId, packets] : m_ReceivedPackets) {
    for (sf::Packet &packet : packets) {
      ID_t packetType;

      try {
        packetType = getPacketType(packet);
      } catch (const std::exception &e) {
        SPDLOG_ERROR("Unable to find packet type");
        continue;
      }

      if (!isPacketRegistered(packetType)) {
        SPDLOG_ERROR("Packet not registered {}", packetType);
        continue;
      }

      if (m_Callbacks.find(packetType) == m_Callbacks.end()) {
        SPDLOG_WARN("Received packet but no callback was assigned {}",
                    packetType);
        continue;
      }

      m_Callbacks.at(packetType)(senderId, packet);
    }
  }
  m_ReceivedPackets.clear();
  m_ReceivedPacketsMutex.unlock();
}
} // namespace Luntik::Networking
