#ifndef PROJECT_NETWORKMANAGER_H
#define PROJECT_NETWORKMANAGER_H

#include "server/Headers/Network/ServerConnection.h"

class NetworkManager
{
 public:
  NetworkManager();
  ~NetworkManager() = default;

  void update();
  void sendDataToClient(GameLib::NetworkPacket& packet, int id);
  void sendDataToClients(GameLib::NetworkPacket& packet);
  void
  sendDataToClientsExcluding(GameLib::NetworkPacket& packet, int exclude_id);
  void
  addObserver(GameLib::Observer* observer, GameLib::EventType observed_event);
  void removeObserver(GameLib::Observer* observer,
                      GameLib::EventType observed_event);

  bool checkServerActive() { return server_connection->isRunning(); };

 private:
  std::unique_ptr<ServerConnection> server_connection = nullptr;
};

#endif // PROJECT_NETWORKMANAGER_H
