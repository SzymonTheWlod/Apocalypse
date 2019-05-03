#include "server/Headers/Characters/Character.h"
#include "gamelib/Headers/Constants.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"
#include "server/Headers/ServerTile.h"
#include <cmath>
#include <fstream>
#include <json.hpp>
#include <server/Headers/Characters/Character.h>

Character::Character(GameLib::CharacterType type) : character(type)
{
  using json = nlohmann::json;
  json config;
  std::ifstream stream;
  stream.open("server/CharacterInfo.json");
  stream >> config;

  config = config[std::to_string(static_cast<int>(type))];

  health_max = config["health"];
  health = config["health"];
  vision_radius = config["vision"];
  armour = config["armour"];
  action_point_max = config["action_points"];
  scout_cost = config["scout_cost"];
  scavenge_cost = config["scavenge_cost"];
  scavenge_amount = config["scavenge_amount"];
  heal_cost = config["heal_cost"];
  heal_amount = config["heal_amount"];
}

// Moves the character if its a valid move request
bool Character::move(ServerTile* tile)
{
  if (!tile->isRevealed())
  {
    return false;
  }

  int cost = 0;
  Point diff = tile->getPosition() - active_tile->getPosition();
  diff.makeAbsolute();
  cost += static_cast<int>(diff.x / tile->getSize());
  cost += static_cast<int>(diff.y / tile->getSize());

  if (useActionPoints(cost))
  {
    position = tile->getPosition();
    active_tile = tile;
    GameLib::NetworkPacket packet(GameLib::EventType::MOVE_CHARACTER);
    packet << character << active_tile->getIndex();
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
    return true;
  }
  return false;
}

// Moves the character even if not valid (for server use)
void Character::forceMove(ServerTile* tile, int id)
{
  position = tile->getPosition();
  active_tile = tile;
  GameLib::NetworkPacket packet(GameLib::EventType::MOVE_CHARACTER);
  packet << character << active_tile->getIndex();
  if (id == -1)
  {
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
  }
  else
  {
    Locator::instance->getNetworkManager()->sendDataToClient(packet, id);
  }
}

void Character::onNotify(GameLib::NetworkPacket& data) {}

bool Character::useActionPoints(const int& amount)
{
  if (action_points - amount >= 0)
  {
    action_points -= amount;
    GameLib::NetworkPacket packet(GameLib::EventType::SET_ACTION_POINTS);
    packet << character << action_points;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
    return true;
  }
  return false;
}

void Character::resetActionPoints()
{
  action_points = action_point_max;
  GameLib::NetworkPacket packet(GameLib::EventType::SET_ACTION_POINTS);
  packet << character << action_points;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void Character::syncToClient(int id)
{
  forceMove(active_tile, id);
  GameLib::NetworkPacket packet(GameLib::EventType::SET_ACTION_POINTS);
  packet << character << action_points;
  Locator::instance->getNetworkManager()->sendDataToClient(packet, id);
  GameLib::NetworkPacket packet2(GameLib::EventType::GIVE_PLAYER_RESOURCES);
  packet2 << character << resources;
  Locator::instance->getNetworkManager()->sendDataToClient(packet2, id);

  // Set the players health to max first, since this sets max hp
  GameLib::NetworkPacket packet3(GameLib::EventType::SET_PLAYER_HEALTH);
  packet3 << character << health_max;
  Locator::instance->getNetworkManager()->sendDataToClient(packet3, id);
  GameLib::NetworkPacket packet4(GameLib::EventType::SET_PLAYER_HEALTH);
  packet4 << character << health;
  Locator::instance->getNetworkManager()->sendDataToClient(packet4, id);
}

void Character::scoutCurrentTile()
{
  if (useActionPoints(scout_cost))
  {
    active_tile->scout();
  }
}

void Character::scavengeCurrentTile()
{
  if (useActionPoints(scavenge_cost))
  {
    Resources res = active_tile->scavenge(scavenge_amount);
    resources = resources + res;
    GameLib::NetworkPacket packet(GameLib::EventType::GIVE_PLAYER_RESOURCES);
    packet << character << res;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
  }
}

void Character::addHealth(float amount)
{
  health += amount;
  if (health < 0)
  {
    health = 0;
  }
  else if (health > health_max)
  {
    health = health_max;
  }
  GameLib::NetworkPacket packet(GameLib::EventType::SET_PLAYER_HEALTH);
  packet << character << health;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void Character::depositResources(Resources amount)
{
  // Take the negative as the player loses what the tile gains
  Resources negative(0, 0, 0, 0, 0, 0);
  negative = negative - amount;
  if (resources.canCombineWith(negative) && active_tile->deposit(amount))
  {
    resources = resources - amount;
    GameLib::NetworkPacket packet(GameLib::EventType::GIVE_PLAYER_RESOURCES);
    packet << character << negative;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
  }
}

void Character::healCharacter(Character* target)
{
  int missing_hp = target->getMissingHP();
  missing_hp /= heal_amount;
  int medicine = resources.med;
  if (missing_hp > medicine)
  {
    missing_hp = medicine;
  }
  if (missing_hp > 0 && useActionPoints(heal_cost))
  {
    resources.med -= missing_hp;
    target->addHealth(static_cast<float>(missing_hp * heal_amount));

    GameLib::NetworkPacket packet(GameLib::EventType::GIVE_PLAYER_RESOURCES);
    packet << character << Resources(0, 0, 0, 0, 0, -missing_hp);
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
  }
}
