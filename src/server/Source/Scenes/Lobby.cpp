#include "server/Headers/Scenes/Lobby.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"

void Lobby::load()
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_CONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_READY_TO_START);
}

Lobby::~Lobby()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_CONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_READY_TO_START);
}

GameLib::SceneType Lobby::update()
{
  return next_scene;
}

void Lobby::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType ::CLIENT_CONNECTED:
    {
      server_data.addClient(data.getClientId());
      server_data.setAllReady(false);
      break;
    }
    case GameLib::EventType::CLIENT_DISCONNECTED:
    {
      server_data.setAllReady(false);
      server_data.removeClient(data.getClientId());
      break;
    }
    case GameLib::EventType ::CLIENT_READY_TO_START:
    {
      server_data.getClientData(data.getClientId()).ready = true;
      break;
    }
    default:
    {
      break;
    }
  }

  GameLib::NetworkPacket packet(GameLib::EventType::NUMBER_OF_CLIENTS_READY);
  packet << server_data.getNumberOfReady();
  packet << server_data.getNumberOfClients();
  Locator::instance->getNetworkManager()->sendDataToClients(packet);

  if (server_data.getNumberOfClients() == server_data.getNumberOfReady())
  {
    next_scene = GameLib::SceneType::CHAR_SELECT_SCENE;
  }
}