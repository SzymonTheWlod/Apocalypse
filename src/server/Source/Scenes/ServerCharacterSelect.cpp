#include "server/Headers/Scenes/ServerCharacterSelect.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"

void ServerCharacterSelect::load()
{
  server_data.setAllReady(false);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SCOUT_CHOSEN);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::MEDIC_CHOSEN);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::BUILDER_CHOSEN);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SOLDIER_CHOSEN);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);

  // Switch the scene on all clients
  GameLib::NetworkPacket new_packet(GameLib::EventType::SERVER_CHANGE_SCENE);
  new_packet << GameLib::SceneType ::CHAR_SELECT_SCENE;
  Locator::instance->getNetworkManager()->sendDataToClients(new_packet);
}

ServerCharacterSelect::~ServerCharacterSelect()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SCOUT_CHOSEN);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::MEDIC_CHOSEN);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::BUILDER_CHOSEN);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SOLDIER_CHOSEN);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
}

GameLib::SceneType ServerCharacterSelect::update()
{
  return next_scene;
}

void ServerCharacterSelect::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType ::SCOUT_CHOSEN:
    {
      selectCharacter(GameLib::CharacterType::SCOUT, data);
      break;
    }
    case GameLib::EventType ::MEDIC_CHOSEN:
    {
      selectCharacter(GameLib::CharacterType::MEDIC, data);
      break;
    }
    case GameLib::EventType ::BUILDER_CHOSEN:
    {
      selectCharacter(GameLib::CharacterType::BUILDER, data);
      break;
    }
    case GameLib::EventType ::SOLDIER_CHOSEN:
    {
      selectCharacter(GameLib::CharacterType::SOLDIER, data);
      break;
    }
    case GameLib::EventType ::CLIENT_DISCONNECTED:
    {
      server_data.setConnected(data.getClientId(), false);
      break;
    }
    case GameLib::EventType ::CLIENT_RECONNECTED:
    {
      server_data.setConnected(data.getClientId(), true);
      // If everyone is connected again, tell clients to continue
      if (!server_data.disconnectedCount())
      {
        GameLib::NetworkPacket packet(GameLib::EventType::CLIENT_RECONNECTED);
        Locator::instance->getNetworkManager()->sendDataToClientsExcluding(
          packet, data.getClientId());
      }
      clientReconnected(data);
      break;
    }
    default:
    {
      return;
    }
  }
}

void ServerCharacterSelect::selectCharacter(
  const GameLib::CharacterType& character, GameLib::NetworkPacket& data)
{
  // If the character is free and this client hasn't already picked a character
  if (!server_data.isCharacterTaken(character) &&
      !server_data.getClientData(data.getClientId()).ready)
  {
    server_data.getClientData(data.getClientId()).type = character;
    server_data.getClientData(data.getClientId()).ready = true;
    // Tell the sending client their choice was valid
    GameLib::NetworkPacket packet(GameLib::EventType::SET_CHARACTER);
    packet << character;
    Locator::instance->getNetworkManager()->sendDataToClient(
      packet, data.getClientId());

    // Tell the other clients that this character is now claimed
    GameLib::NetworkPacket packet2(GameLib::EventType::CLAIM_CHARACTER);
    packet2 << character;
    Locator::instance->getNetworkManager()->sendDataToClientsExcluding(
      packet2, data.getClientId());

    // If everyone has chosen a character, move onto the game scene
    if (server_data.getNumberOfReady() == server_data.getNumberOfClients())
    {
      next_scene = GameLib::SceneType ::GAME_SCENE;
    }
  }
}

void ServerCharacterSelect::clientReconnected(GameLib::NetworkPacket& data)
{
  // Change the clients scene to the correct one
  GameLib::NetworkPacket packet(GameLib::EventType::SERVER_CHANGE_SCENE);
  packet << GameLib::SceneType ::CHAR_SELECT_SCENE;
  Locator::instance->getNetworkManager()->sendDataToClient(packet,
                                                           data.getClientId());

  for (auto& [key, value] : server_data.getData())
  {
    // Check through every id to see if a character has been selected
    if (value.type != GameLib::CharacterType::NONE)
    {
      // Find if the disconnected client had already chosen and set it again
      if (key == data.getClientId())
      {
        GameLib::NetworkPacket packet2(GameLib::EventType::SET_CHARACTER);
        packet2 << value.type;
        Locator::instance->getNetworkManager()->sendDataToClient(
          packet2, data.getClientId());
      }
      else
      {
        GameLib::NetworkPacket packet2(GameLib::EventType::CLAIM_CHARACTER);
        packet2 << value.type;
        Locator::instance->getNetworkManager()->sendDataToClient(
          packet2, data.getClientId());
      }
    }
  }
}