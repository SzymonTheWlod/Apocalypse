
#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include <iostream>
#include <json.hpp>
#include <random>

#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Printer.h"
#include "gamelib/Headers/Resources.h"
#include "gamelib/Headers/Subject.h"
#include "server/Headers/ServerTile.h"

using json = nlohmann::json;

class Map : public GameLib::Subject
{
 public:
  Map() = default;
  ~Map() = default;

  std::vector<ServerTile>& getMap() { return tiles; }
  ServerTile& getTile(size_t index) { return tiles[index]; }
  bool load();

  unsigned long long& getWidth() { return map_width; }
  unsigned long long& getHeight() { return map_height; }

  void sendMapToID(int id);

  void setTileNeighbours();
  void revealAroundTile(size_t tile_index, float fade, int radius);

  ServerTile* getTownHall() { return town_hall; };

  void damageTilesInLine(Point line_start,
                         Point line_end,
                         const float& damage,
                         const float& radius);
  float tile_size = 0;

 private:
  void distributeTiles(json& json_file);
  void populateTileTypes();
  void positionTiles();
  void generateMap();
  void sendMap();

  void setMinResources(json& json_file);
  void distributeResources(json& json_file, int& resource, int& pool);

  int num_tiles = 0;

  int max_num_buildings = 0;
  unsigned long long map_width = 0;
  unsigned long long map_height = 0;

  Resources resource_pool;
  int resource_pool_total = 0;
  Resources cur_resources;

  std::mt19937 generator;
  std::uniform_int_distribution<int> probability_roll;

  std::map<GameLib::TileType, int> num_buildings;

  std::vector<GameLib::TileType> tiletypes;
  std::vector<ServerTile> tiles;

  ServerTile* town_hall = nullptr;
};

#endif // PROJECT_MAP_H
