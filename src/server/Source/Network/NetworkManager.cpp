#include "server/Headers/Network/NetworkManager.h"

// A public interface to the network connection, allows other objects to listen
// for network events and send data to the clients
NetworkManager::NetworkManager() :
  server_connection(std::make_unique<ServerConnection>())
{
  server_connection->initialise();
  server_connection->start();
}

void NetworkManager::update()
{
  server_connection->processClientMessages();
}

void NetworkManager::sendDataToClients(GameLib::NetworkPacket& packet)
{
  server_connection->sendDataToClients(packet);
}

void NetworkManager::sendDataToClient(GameLib::NetworkPacket& packet, int id)
{
  server_connection->sendDataToClient(packet, id);
}

void NetworkManager::sendDataToClientsExcluding(GameLib::NetworkPacket& packet,
                                                int exclude_id)
{
  server_connection->sendDataToClientsExcluding(packet, exclude_id);
}

void NetworkManager::addObserver(GameLib::Observer* observer,
                                 GameLib::EventType observed_event)
{
  server_connection->addObserver(observer, observed_event);
}

void NetworkManager::removeObserver(GameLib::Observer* observer,
                                    GameLib::EventType observed_event)
{
  server_connection->removeObserver(observer, observed_event);
}