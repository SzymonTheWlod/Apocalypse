#include "client/Headers/Network/NetworkManager.h"

// A public interface to the NetworkConnection, allows other objects to listen
// for network events and send data to the server
NetworkManager::NetworkManager() :
  client_connection(std::make_unique<ClientConnection>())
{
  client_connection->initialise();
}

void NetworkManager::update()
{
  client_connection->processServerMessages();
}

void NetworkManager::sendDataToServer(GameLib::NetworkPacket& packet)
{
  client_connection->sendDataToServer(packet);
}

void NetworkManager::addObserver(GameLib::Observer* observer,
                                 GameLib::EventType observed_event)
{
  client_connection->addObserver(observer, observed_event);
}

void NetworkManager::removeObserver(GameLib::Observer* observer,
                                    GameLib::EventType observed_event)
{
  client_connection->removeObserver(observer, observed_event);
}

void NetworkManager::tryConnection(const char* ip)
{
  client_connection->disconnect();
  client_connection->connectToServer(ip);
}