#ifndef PROJECT_SERVERTILE_H
#define PROJECT_SERVERTILE_H

#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Point.h"
#include "gamelib/Headers/Resources.h"
#include <string>

class ServerTile
{
 public:
  explicit ServerTile(GameLib::TileType tile_type, size_t index_in_vector);

  ServerTile() = default;
  ~ServerTile() = default;

  Resources& getResources() { return resources; };
  void setResources(Resources& new_amount) { resources = new_amount; };
  Point getPosition() { return position; };
  void setPosition(Point new_pos) { position = new_pos; };

  GameLib::TileType getType() { return building_type; };
  size_t getIndex() { return index; };
  void setSize(float new_size) { size = new_size; };
  float getSize() { return size; };

  void scout();
  bool isScouted() { return scouted; };

  void damage(const float& damage);

  Resources scavenge(int amount);
  bool deposit(Resources amount);

  void setFade(float new_fade) { fade = new_fade; };
  float getFade() { return fade; };
  bool isRevealed() { return fade != 1; };

  void addNeighbour(size_t tile_index) { neighbours.push_back(tile_index); };
  std::vector<size_t> getNeighbours() { return neighbours; };

  void setHit(bool is_hit) { hit = is_hit; };
  bool getHit() { return hit; };

  float getHealth() { return health; };

 private:
  float size = 0;

  GameLib::TileType building_type = GameLib::TileType::NONE;
  Point position = Point(0, 0);

  float health = 100;

  bool destroyed = false;
  bool item = false;

  Resources resources;
  size_t index = 0;
  std::vector<size_t> neighbours;

  bool scouted = false;
  float fade = 1;

  bool hit = false;
};

#endif // PROJECT_SERVERTILE_H
