
#include <server/Headers/ServerTile.h>

#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Printer.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"
#include "server/Headers/ServerTile.h"

ServerTile::ServerTile(GameLib::TileType tile_type, size_t index_in_vector) :
  building_type(tile_type), index(index_in_vector)
{
  if (tile_type == GameLib::TileType::GRASS ||
      tile_type == GameLib::TileType::TOWN_HALL)
  {
    health = 1000000;
  }
}

void ServerTile::scout()
{
  scouted = true;
  GameLib::NetworkPacket packet(GameLib::EventType::SET_SCOUTED);
  packet << index;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

Resources ServerTile::scavenge(int amount)
{
  Resources return_res = resources.takeDistribution(amount);
  GameLib::NetworkPacket packet(GameLib::EventType::SET_TILE_RESOURCES);
  packet << index << resources;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
  return return_res;
}

void ServerTile::damage(const float& damage)
{
  health -= damage;
  if (health < 0)
  {
    health = 0;
    // Destroy the remaining resources
    resources = resources - resources;
    GameLib::NetworkPacket packet(GameLib::EventType::SET_TILE_RESOURCES);
    packet << index << resources;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
  }
  GameLib::NetworkPacket packet(GameLib::EventType::SET_TILE_HEALTH);
  packet << index << health;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

bool ServerTile::deposit(Resources amount)
{
  if (resources.canCombineWith(amount))
  {
    resources = resources + amount;
    GameLib::NetworkPacket packet(GameLib::EventType::SET_TILE_RESOURCES);
    packet << index << resources;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);
    return true;
  }
  return false;
}
