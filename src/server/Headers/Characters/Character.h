#ifndef PROJECT_CHARMANAGER_H
#define PROJECT_CHARMANAGER_H

#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/NetworkPacket.h"
#include "gamelib/Headers/Observer.h"
#include "gamelib/Headers/Point.h"
#include "server/Headers/ServerTile.h"
#include <iostream>

class Character : public GameLib::Observer
{
 public:
  Character() = default;
  explicit Character(GameLib::CharacterType type);
  ~Character() override = default;

  void onNotify(GameLib::NetworkPacket& data) override;
  bool move(ServerTile* tile);
  void forceMove(ServerTile* tile, int id = -1);
  void setActionPoints(const int& amount) { action_points = amount; };
  bool useActionPoints(const int& amount);
  void resetActionPoints();

  void addHealth(float amount);
  float getHealth() { return health; };
  float getMaxHealth() { return health_max; };

  Point getPosition() { return position; };

  void setTile(ServerTile* new_tile) { active_tile = new_tile; };
  ServerTile* getTile() { return active_tile; };

  void scoutCurrentTile();
  void scavengeCurrentTile();
  void depositResources(Resources amount);

  int getVisionRadius() { return vision_radius; };

  void syncToClient(int id);
  void healCharacter(Character* target);

  int getMissingHP() { return static_cast<int>(health_max - health); };

 protected:
  GameLib::CharacterType character = GameLib::CharacterType::NONE;
  int action_point_max = 10;
  float health_max = 0;

  int vision_radius = 1;
  int scout_cost = 1;

  int scavenge_cost = 2;
  int scavenge_amount = 10;

  int heal_cost = 0;
  int heal_amount = 10;

 private:
  Resources resources;
  Point position = Point(0, 0);
  float health = 0;
  int armour = 0;
  int action_points = 0;

  ServerTile* active_tile = nullptr;
};

#endif // PROJECT_CHARMANAGER_H