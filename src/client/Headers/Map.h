
#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include <iostream>
#include <random>

#include "client/Headers/Tile.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Observer.h"

class Map : public GameLib::Observer
{
 public:
  Map();
  ~Map() override;

  bool load(GameLib::NetworkPacket& data);
  void onNotify(GameLib::NetworkPacket& data) override;
  void render(double delta_time);
  void update();

  Tile* checkIfClicked(Point click_pos);
  Tile* getTile(const size_t& index) { return tiles[index].get(); };
  Tile* getTownHall() { return town_hall; };

 private:
  unsigned long long num_tiles = 0;
  unsigned long long map_width = 0;
  unsigned long long map_height = 0;

  bool loaded = false;
  std::vector<std::unique_ptr<Tile>> tiles;

  Tile* town_hall = nullptr;
};

#endif // PROJECT_MAP_H
