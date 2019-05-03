#ifndef PROJECT_NETWORKMANAGER_H
#define PROJECT_NETWORKMANAGER_H

#include "ClientConnection.h"
#include "gamelib/Headers/Subject.h"

class NetworkManager
{
 public:
  NetworkManager();
  ~NetworkManager() = default;

  void update();
  void sendDataToServer(GameLib::NetworkPacket& packet);

  void
  addObserver(GameLib::Observer* observer, GameLib::EventType observed_event);
  void removeObserver(GameLib::Observer* observer,
                      GameLib::EventType observed_event);

  void tryConnection(const char* ip);

  void disconnect() { client_connection->disconnect(); };

  bool isConnected() { return client_connection->isConnected(); };

 private:
  std::unique_ptr<ClientConnection> client_connection = nullptr;
};

#endif // PROJECT_NETWORKMANAGER_H
