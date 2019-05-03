#include <client/Headers/Characters/Character.h>

#include "client/Headers/AudioManager.h"
#include "client/Headers/Characters/Character.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
void Character::render(double delta_time)
{
  if (highlighted)
  {
    highlight->render(delta_time);
  }
  sprite->render(delta_time);
}

void Character::setPosition(const Point& pos)
{
  position = pos;
  sprite->xPos(pos.x);
  sprite->yPos(pos.y);
  highlight->xPos(pos.x);
  highlight->yPos(pos.y);
  click.setPosition(position);
}

void Character::moveTo(Tile* tile)
{
  setTile(tile);
  setPosition(tile->getPosition());
  click.setPosition(position);
  Locator::instance->getAudioManager()->playSoundFX(GameLib::SoundFX::MOVE);
}

// Sends a move request to the server
void Character::requestMove(Tile* tile)
{
  GameLib::NetworkPacket packet(GameLib::EventType::MOVE_CHARACTER);
  packet << character_type << tile->getIndex();
  Locator::instance->getNetworkManager()->sendDataToServer(packet);
}
void Character::setTile(Tile* new_tile)
{
  if (active_tile)
  {
    active_tile->removeCharacter();
  }
  active_tile = new_tile;
  active_tile->addCharacter();
}

void Character::requestScout()
{
  GameLib::NetworkPacket packet(GameLib::EventType::SCOUT_TILE);
  packet << character_type;
  Locator::instance->getNetworkManager()->sendDataToServer(packet);
}

void Character::requestScavenge()
{
  GameLib::NetworkPacket packet(GameLib::EventType::SCAVENGE_TILE);
  packet << character_type;
  Locator::instance->getNetworkManager()->sendDataToServer(packet);
}

void Character::addResources(Resources& new_resources)
{
  resources = resources + new_resources;
}

bool Character::checkForClick(Point pos)
{
  return click.isPointInArea(pos);
}

void Character::setHealth(float new_health)
{
  if (new_health > health)
  {
    Locator::instance->getAudioManager()->playSoundFX(GameLib::SoundFX::HEAL);
  }
  health = new_health;
  if (health > max_health)
  {
    max_health = health;
  }
}

void Character::requestDeposit(const Resources& amount)
{
  GameLib::NetworkPacket packet(GameLib::EventType::DEPOSIT_RESOURCES);
  packet << character_type << amount;
  Locator::instance->getNetworkManager()->sendDataToServer(packet);
}

void Character::setHighlighted(bool flag)
{
  if (!lock_highlight)
  {
    highlighted = flag;
  }
}

void Character::setWidth(const float& width)
{
  sprite->setWidth(width);
  highlight->setWidth(width);
  click.setWidth(width);
}

void Character::setHeight(const float& height)
{
  sprite->setHeight(height);
  highlight->setHeight(height);
  click.setHeight(height);
}