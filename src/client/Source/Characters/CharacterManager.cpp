#include <client/Headers/Characters/CharacterManager.h>

#include "client/Headers/AudioManager.h"
#include "client/Headers/Characters/CharacterManager.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
CharacterManager::CharacterManager()
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CREATE_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::MOVE_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_ACTION_POINTS);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::GIVE_PLAYER_RESOURCES);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_PLAYER_HEALTH);

  using pair = std::pair<GameLib::CharacterType, std::unique_ptr<Character>>;
  characters.insert(pair(GameLib::CharacterType::SOLDIER, nullptr));
  characters.insert(pair(GameLib::CharacterType::MEDIC, nullptr));
  characters.insert(pair(GameLib::CharacterType::SCOUT, nullptr));
  characters.insert(pair(GameLib::CharacterType::BUILDER, nullptr));

  your_turn =
    std::make_unique<AnimatedSprite>("data/YourTurn.png", false, false);
  your_turn->xPos(350);
  your_turn->yPos(200);
}

CharacterManager::~CharacterManager()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CREATE_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::MOVE_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_ACTION_POINTS);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::GIVE_PLAYER_RESOURCES);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_PLAYER_HEALTH);
}

void CharacterManager::createCharacter(GameLib::CharacterType character)
{
  // If the character hasn't been created yet
  if (!characters[character])
  {
    // Create a new character depending on the type
    switch (character)
    {
      case GameLib::CharacterType::SCOUT:
      {
        characters[character] = std::make_unique<ClientScout>();
        break;
      }
      case GameLib::CharacterType::BUILDER:
      {
        characters[character] = std::make_unique<ClientBuilder>();
        break;
      }
      case GameLib::CharacterType::MEDIC:
      {
        characters[character] = std::make_unique<ClientMedic>();
        break;
      }
      case GameLib::CharacterType::SOLDIER:
      {
        characters[character] = std::make_unique<ClientSolider>();
        break;
      }
      default:
        break;
    }
    characters[character]->setTile(map->getTile(0));
    characters[character]->setWidth(map->getTile(0)->getSize() / 2);
    characters[character]->setHeight(map->getTile(0)->getSize() / 2);
    position_priority++;
  }
}

// Moves the selected character, this method should be triggered by a server
// event
void CharacterManager::moveCharacter(GameLib::CharacterType character,
                                     Tile* tile)
{
  if (characters[character])
  {
    characters[character]->moveTo(tile);
  }
}

void CharacterManager::render(double delta_time)
{
  for (auto& [key, val] : characters)
  {
    if (val)
    {
      val->setPosition(val->getTile()->getNextRenderPos());
      val->render(delta_time);
    }
  }

  for (auto& [key, val] : characters)
  {
    if (val)
    {
      val->getTile()->resetRenderPos();
    }
  }

  if (show_text)
  {
    your_turn->render(delta_time);
  }
}

void CharacterManager::onNotify(GameLib::NetworkPacket& data)
{
  GameLib::CharacterType type;
  data >> type;
  switch (data.getType())
  {
    case GameLib::EventType ::CREATE_CHARACTER:
    {
      createCharacter(type);
      break;
    }
    case GameLib::EventType ::MOVE_CHARACTER:
    {
      size_t index;
      data >> index;
      moveCharacter(type, map->getTile(index));
      break;
    }
    case GameLib::EventType ::SET_ACTION_POINTS:
    {
      int amount;
      data >> amount;

      if (local_character == type &&
          amount > characters[type]->getActionPoints())
      {
        Locator::instance->getAudioManager()->enableFX(true);
        time_elapsed = 0;
        show_text = true;
        Locator::instance->getAudioManager()->playSoundFX(
          GameLib::SoundFX::YOUR_TURN);
      }
      characters[type]->setActionPoints(amount);
      if (characters[type]->getActionPoints() == 0)
      {
        GameLib::NetworkPacket packet(GameLib::EventType::END_TURN);
        packet << type;
        Locator::instance->getNetworkManager()->sendDataToServer(packet);
      }
      break;
    }
    case GameLib::EventType::GIVE_PLAYER_RESOURCES:
    {
      Resources res;
      data >> res;
      characters[type]->addResources(res);
      break;
    }
    case GameLib::EventType ::SET_PLAYER_HEALTH:
    {
      float health;
      data >> health;
      characters[type]->setHealth(health);
      break;
    }
    default:
    {
      break;
    }
  }

  if (type == local_character)
  {
    characters[local_character]->setHighlightColour(ASGE::COLOURS::GOLD);
  }
}

void CharacterManager::setLocalCharacter(GameLib::CharacterType type)
{
  local_character = type;
}

Character* CharacterManager::checkPlayerClicked(Point clicked_pos)
{
  for (auto& [key, val] : characters)
  {
    if (val && val->checkForClick(clicked_pos))
    {
      return val.get();
    }
  }
  return nullptr;
}

void CharacterManager::update(double delta_time)
{
  if (show_text)
  {
    time_elapsed += delta_time;
    if (time_elapsed > 1500)
    {
      show_text = false;
    }
  }

  Point mouse = Locator::instance->getInputManager()->getWorldMousePosition();
  for (auto& [key, val] : characters)
  {
    if (val)
    {
      val->setHighlighted(val->checkForClick(mouse));
      // Always highlight the player if its their turn
      if (key == local_character && val->getActionPoints() > 0)
      {
        val->setHighlighted(true);
      }
    }
  }
}