#ifndef PROJECT_CLIENTCHARACTERMANAGER_H
#define PROJECT_CLIENTCHARACTERMANAGER_H

#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Tile.h"
#include "client/Headers/UI/ClickArea.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/NetworkPacket.h"
#include <Engine/Renderer.h>
#include <string>

class Character
{
 public:
  Character() = default;
  virtual ~Character() = default;

  void render(double delta_time);
  void moveTo(Tile* tile);
  void requestMove(Tile* tile);
  void requestScout();
  void requestScavenge();
  void requestDeposit(const Resources& amount);

  void setActionPoints(const int& amount) { action_points = amount; };
  int getActionPoints() { return action_points; };

  bool checkForClick(Point pos);

  void setTile(Tile* new_tile);
  Tile* getTile() { return active_tile; };

  void setPosition(const Point& pos);

  void setWidth(const float& width);
  void setHeight(const float& height);

  GameLib::CharacterType getCharacterType() { return character_type; };

  void addResources(Resources& new_resources);
  Resources& getResources() { return resources; };
  void setHealth(float new_health);
  float getHealthPercent() { return health / max_health; };

  void setHighlighted(bool flag);
  void setHighlightLock(bool flag) { lock_highlight = flag; };
  void setHighlightColour(const ASGE::Colour& colour)
  {
    highlight->setColour(colour);
  }

 protected:
  Resources resources;

  Point position = Point(0, 0);

  std::unique_ptr<AnimatedSprite> sprite = nullptr;
  std::unique_ptr<AnimatedSprite> highlight = nullptr;
  ClickArea click;

  GameLib::CharacterType character_type = GameLib::CharacterType::NONE;

  Tile* active_tile = nullptr;
  int action_points = 0;
  float health = 0;
  float max_health = 0;

  bool highlighted = false;
  bool lock_highlight = false;
};

#endif // PROJECT_CLIENTCHARACTERMANAGER_H
