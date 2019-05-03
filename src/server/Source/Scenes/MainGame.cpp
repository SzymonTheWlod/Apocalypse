#include "server/Headers/Scenes/MainGame.h"
#include "gamelib/Headers/Constants.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"

MainGame::MainGame(PersistentServerData& data) : ServerScene(data)
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::END_TURN);
}

MainGame::~MainGame()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::END_TURN);
}

void MainGame::load()
{
  // Switch the scene on all clients
  GameLib::NetworkPacket new_packet(GameLib::EventType::SERVER_CHANGE_SCENE);
  new_packet << GameLib::SceneType ::GAME_SCENE;
  Locator::instance->getNetworkManager()->sendDataToClients(new_packet);

  main_map.load();
  character_manager.setMap(&main_map);
  event_manager.setMap(&main_map);
  character_manager.setEventManager(&event_manager);

  // Create a character for each player
  for (auto& [key, val] : server_data.getData())
  {
    // Also send a packet to the client to create the players locally
    GameLib::NetworkPacket packet(GameLib::EventType::CREATE_CHARACTER);
    packet << val.type;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
    character_manager.addCharacter(val.type);
  }
  character_manager.calculateWinCondition();
  character_manager.nextTurn(GameLib::CharacterType::MEDIC);

  character_manager.addObserver(this, GameLib::EventType::SIGNAL_EXIT);
}

GameLib::SceneType MainGame::update()
{
  if (game_ending)
  {
    time_elapsed += 0.01f;
    if (time_elapsed > 5)
    {
      GameLib::NetworkPacket packet(GameLib::EventType::SERVER_CHANGE_SCENE);
      packet << GameLib::SceneType ::MENU_SCENE;
      Locator::instance->getNetworkManager()->sendDataToClients(packet);
      next_scene = GameLib::SceneType::MENU_SCENE;
    }
  }
  return next_scene;
}

void MainGame::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType::END_TURN:
    {
      break;
    }
    case GameLib::EventType ::CLIENT_DISCONNECTED:
    {
      server_data.setConnected(data.getClientId(), false);
      break;
    }
    case GameLib::EventType ::CLIENT_RECONNECTED:
    {
      if (!server_data.getClientData(data.getClientId()).connected)
      {
        clientReconnected(data);
        server_data.setConnected(data.getClientId(), true);

        // If everyone is connected again, tell clients to continue
        if (!server_data.disconnectedCount())
        {
          GameLib::NetworkPacket packet(GameLib::EventType::CLIENT_RECONNECTED);
          Locator::instance->getNetworkManager()->sendDataToClientsExcluding(
            packet, data.getClientId());
        }
      }
      break;
    }
    case GameLib::EventType ::SIGNAL_EXIT:
    {
      game_ending = true;
      break;
    }
    default:
    {
      break;
    }
  }
}

void MainGame::clientReconnected(GameLib::NetworkPacket& data)
{
  // Change the clients scene to the correct one
  GameLib::NetworkPacket packet(GameLib::EventType::SERVER_CHANGE_SCENE);
  packet << GameLib::SceneType::GAME_SCENE;
  Locator::instance->getNetworkManager()->sendDataToClient(packet,
                                                           data.getClientId());

  // Set the clients local character
  GameLib::NetworkPacket packet2(GameLib::EventType::SET_CHARACTER);
  packet2 << server_data.getClientData(data.getClientId()).type;
  Locator::instance->getNetworkManager()->sendDataToClient(packet2,
                                                           data.getClientId());

  // Send the map again
  main_map.sendMapToID(data.getClientId());

  // Spawn in all of the characters
  for (auto& [key, value] : server_data.getData())
  {
    GameLib::NetworkPacket packet3(GameLib::EventType::CREATE_CHARACTER);
    packet3 << value.type;
    Locator::instance->getNetworkManager()->sendDataToClient(
      packet3, data.getClientId());
  }

  // Sync the character data
  character_manager.syncAllData(data.getClientId());

  event_manager.syncData();
}