#ifndef PROJECT_TILE_H
#define PROJECT_TILE_H

#include "AnimatedSprite.h"
#include "client/Headers/UI/ClickArea.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Observer.h"
#include "gamelib/Headers/Resources.h"
#include <Engine/Renderer.h>
#include <json.hpp>
#include <string>

class Tile
{
 public:
  Tile(GameLib::TileType& type,
       float& size,
       size_t index_in_vector,
       const nlohmann::json& config);
  Tile(const Tile&) = delete;
  Tile& operator=(const Tile&) = delete;

  Tile() = default;
  ~Tile() = default;

  void generateTileTypes(GameLib::TileType& type,
                         float& size,
                         const nlohmann::json& config);

  bool checkForClick(Point position);
  void render(double delta_time);

  void setPosition(Point pos);

  Point getPosition() { return position; };
  GameLib::TileType getType() { return building_type; };

  float getSize() { return tile_sprite->width(); };
  std::string& getName();

  Resources& getResources() { return resources_on_tile; };
  void setResources(Resources& new_res) { resources_on_tile = new_res; };

  float& getHealth();
  float getHealthPercent() { return health / max_health; };
  void setHealth(const float& new_health);
  size_t getIndex() { return index; };

  void addCharacter() { characters_on_tile++; };
  void removeCharacter() { characters_on_tile--; };

  Point getNextRenderPos();
  void resetRenderPos() { render_pos = 0; };

  void setScouted(bool flag) { scouted = flag; };
  bool isScouted() { return scouted; };

  void setFade(const float& fade);
  bool isRevealed() { return mask_sprite->getOpacity() != 1; };

  void setHighlighted(bool flag);
  void setHighlightLock(bool flag) { lock_highlight = flag; };

 private:
  std::string name;
  float max_health = 100;
  float health = 100;
  Point position = Point(0, 0);

  GameLib::TileType building_type = GameLib::TileType ::NONE;

  Resources resources_on_tile;
  int bandits = 0;

  bool destroyed = false;
  bool item = false;
  bool scouted = false;

  ClickArea click;

  std::unique_ptr<AnimatedSprite> tile_sprite = nullptr;
  std::unique_ptr<AnimatedSprite> mask_sprite = nullptr;
  std::unique_ptr<AnimatedSprite> highlight_sprite = nullptr;

  bool highlighted = false;
  bool lock_highlight = false;

  size_t index = 0;
  int characters_on_tile = 0;
  int render_pos = 0;
};

#endif // PROJECT_TILE_H
