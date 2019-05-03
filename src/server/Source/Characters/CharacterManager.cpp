
#include <server/Headers/Characters/CharacterManager.h>

#include "server/Headers/Characters/CharacterManager.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"

CharacterManager::CharacterManager()
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::MOVE_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::END_TURN);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SCOUT_TILE);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SCAVENGE_TILE);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType ::BUILD_BUNKER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::DEPOSIT_RESOURCES);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::HEAL_PLAYER);
}

CharacterManager::~CharacterManager()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::MOVE_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::END_TURN);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SCOUT_TILE);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SCAVENGE_TILE);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::BUILD_BUNKER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::DEPOSIT_RESOURCES);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::HEAL_PLAYER);
}

void CharacterManager::addCharacter(GameLib::CharacterType character)
{
  if (!characters.count(character))
  {
    using pair = std::pair<GameLib::CharacterType, Character>;
    switch (character)
    {
      case GameLib::CharacterType::SCOUT:
      {
        characters.insert(pair(character, ServerScout()));
        break;
      }
      case GameLib::CharacterType::BUILDER:
      {
        characters.insert(pair(character, ServerBuilder()));
        break;
      }
      case GameLib::CharacterType::MEDIC:
      {
        characters.insert(pair(character, ServerMedic()));
        break;
      }
      case GameLib::CharacterType::SOLDIER:
      {
        characters.insert(pair(character, ServerSoldier()));
        break;
      }
      default:
        break;
    }
    characters[character].forceMove(map->getTownHall());
    updateFogOfWar(map->getTownHall()->getIndex(), character);
    map->getTownHall()->scout();
    characters[character].addHealth(characters[character].getMaxHealth());
  }
}

void CharacterManager::nextTurn(GameLib::CharacterType character)
{
  bool wrapped_around = false;
  if (character == players_turn)
  {
    if (characters.count(character))
    {
      characters[players_turn].setActionPoints(0);
    }
    int count = 0;
    do
    {
      if (count > 6)
      {
        // All players have 0 health, so players lose
        GameLib::NetworkPacket packet(GameLib::EventType::LOSE_GAME);
        Locator::instance->getNetworkManager()->sendDataToClients(packet);
        GameLib::NetworkPacket packet2(GameLib::EventType::SIGNAL_EXIT);
        notify(packet2.getType(), packet2);
        return;
      }
      count++;
      if (static_cast<int>(players_turn) == 3)
      {
        wrapped_around = true;
        players_turn = GameLib::CharacterType::MEDIC;
      }
      else
      {
        players_turn = static_cast<GameLib::CharacterType>(
          static_cast<int>(players_turn) + 1);
      }
    } while (!characters.count(players_turn) ||
             characters[players_turn].getHealth() <= 0);

    if (wrapped_around)
    {
      event_manager->turnEnded();
    }
    characters[players_turn].resetActionPoints();
  }
}

void CharacterManager::onNotify(GameLib::NetworkPacket& data)
{
  GameLib::CharacterType type;
  data >> type;
  switch (data.getType())
  {
    case GameLib::EventType ::MOVE_CHARACTER:
    {
      size_t index;
      data >> index;
      size_t prev_index = characters[type].getTile()->getIndex();
      if (characters[type].move(&map->getTile(index)))
      {
        updateFogOfWar(prev_index, type);
      }
      break;
    }
    case GameLib::EventType ::END_TURN:
    {
      nextTurn(type);
      break;
    }
    case GameLib::EventType ::SCOUT_TILE:
    {
      characters[type].scoutCurrentTile();
      break;
    }
    case GameLib::EventType ::SCAVENGE_TILE:
    {
      characters[type].scavengeCurrentTile();
      break;
    }
    case GameLib::EventType ::DEPOSIT_RESOURCES:
    {
      if (characters[type].getTile() == map->getTownHall())
      {
        Resources amount;
        data >> amount;
        characters[type].depositResources(amount);
      }
      break;
    }
    case GameLib::EventType ::HEAL_PLAYER:
    {
      GameLib::CharacterType type2;
      data >> type2;
      if (characters[type].getTile() == characters[type2].getTile())
      {
        characters[type].healCharacter(&characters[type2]);
      }
      break;
    }
    default:
    {
      tileEvents(data);
      break;
    }
  }
}

void CharacterManager::tileEvents(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::SET_TILE_HEALTH)
  {
    damagePlayersOnTile(data);
  }
  else if (data.getType() == GameLib::EventType::BUILD_BUNKER)
  {
    checkForWin();
  }
}

void CharacterManager::damagePlayersOnTile(GameLib::NetworkPacket& data)
{
  size_t index;
  float damage;
  data >> index >> damage;
  for (auto& [key, value] : characters)
  {
    if (value.getTile()->getIndex() == index)
    {
      value.addHealth(-damage);
    }
  }
}

void CharacterManager::updateFogOfWar(const size_t& start_tile,
                                      const GameLib::CharacterType& character)
{
  // Fade out around the start point
  map->revealAroundTile(
    start_tile, 0.5f, characters[character].getVisionRadius());
  // Fade in all the other characters
  for (auto& [key, value] : characters)
  {
    map->revealAroundTile(
      value.getTile()->getIndex(), 0, value.getVisionRadius());
  }
}

void CharacterManager::syncAllData(int id)
{
  for (auto& [key, value] : characters)
  {
    value.syncToClient(id);
  }

  GameLib::NetworkPacket packet(GameLib::EventType::SET_WIN_CONDITION);
  packet << win_amount;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void CharacterManager::setMap(Map* new_map)
{
  map = new_map;
  map->addObserver(this, GameLib::EventType::SET_TILE_HEALTH);
}

void CharacterManager::checkForWin()
{
  // Check to see if everyone is on the town hall
  bool same_tile = true;
  ServerTile* tile = nullptr;
  for (auto& [key, value] : characters)
  {
    if (tile && tile != value.getTile())
    {
      same_tile = false;
      break;
    }
    else
    {
      tile = value.getTile();
    }
  }

  // If everyone isn't on the town hall, or this tile isn't the town hall,
  // return early
  if (!same_tile || tile != map->getTownHall())
  {
    return;
  }

  if (map->getTownHall()->getResources() >= win_amount)
  {
    GameLib::NetworkPacket packet(GameLib::EventType::WIN_GAME);
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
    GameLib::NetworkPacket packet2(GameLib::EventType::SIGNAL_EXIT);
    notify(packet2.getType(), packet2);
  }
}

void CharacterManager::calculateWinCondition()
{
  Resources increment_win_amount(40, 40, 25, 25, 8, 5);

  for (unsigned long long i = characters.size(); i > 0; i--)
  {
    win_amount = win_amount + increment_win_amount;
  }

  GameLib::NetworkPacket packet(GameLib::EventType::SET_WIN_CONDITION);
  packet << win_amount;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}
