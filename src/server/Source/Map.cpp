#include <fstream>
#include <server/Headers/Map.h>

#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"

bool Map::load()
{
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  generator = std::mt19937(seed);
  probability_roll = std::uniform_int_distribution<int>(1, 100);

  GameLib::TileType first = GameLib::TileType::TOWN_HALL;
  GameLib::TileType last = GameLib::TileType::SUPERMARKET;
  for (int i = static_cast<int>(first); i <= static_cast<int>(last); i++)
  {
    using pair = std::pair<GameLib::TileType, int>;
    num_buildings.insert(pair(static_cast<GameLib::TileType>(i), 0));
  }

  json json_file;

  std::ifstream stream;
  stream.open("server/BuildingInfo.json");

  stream >> json_file;

  max_num_buildings = json_file["config"]["buildings"];
  map_height = json_file["config"]["map_height"];
  map_width = json_file["config"]["map_width"];
  tile_size = json_file["config"]["tile_width"];

  // Set the minimum number of buildings
  for (const json& building : json_file["building_number"])
  {
    num_buildings[static_cast<GameLib::TileType>(building["type"])] =
      building["min"];
    num_tiles += static_cast<int>(building["min"]);
  }

  // Randomly distribute buildings till the max tiles limit is hit
  distributeTiles(json_file);
  // Create all of the tile types
  populateTileTypes();
  // Create the tiles from the tile types
  generateMap();
  // Position the tiles
  positionTiles();

  resource_pool.water = json_file["resources"]["water"];
  resource_pool.food = json_file["resources"]["food"];
  resource_pool.wood = json_file["resources"]["wood"];
  resource_pool.metal = json_file["resources"]["metal"];
  resource_pool.med = json_file["resources"]["medicine"];
  resource_pool.fuel = json_file["resources"]["fuel"];
  resource_pool_total = resource_pool.total();

  setMinResources(json_file);

  while (resource_pool_total > 0)
  {
    for (ServerTile& tile : tiles)
    {
      std::string building_string =
        json_file["id_to_name"]
                 [std::to_string(static_cast<int>(tile.getType()))];
      distributeResources(json_file["buildings"][building_string]["water"],
                          tile.getResources().water,
                          resource_pool.water);
      distributeResources(json_file["buildings"][building_string]["food"],
                          tile.getResources().food,
                          resource_pool.food);
      distributeResources(json_file["buildings"][building_string]["wood"],
                          tile.getResources().wood,
                          resource_pool.wood);
      distributeResources(json_file["buildings"][building_string]["metal"],
                          tile.getResources().metal,
                          resource_pool.metal);
      distributeResources(json_file["buildings"][building_string]["medicine"],
                          tile.getResources().med,
                          resource_pool.med);
      distributeResources(json_file["buildings"][building_string]["fuel"],
                          tile.getResources().fuel,
                          resource_pool.fuel);
    }
  }
  setTileNeighbours();
  // Send the map to clients
  sendMap();
  // Reveal the starting area
  for (ServerTile& tile : tiles)
  {
    if (tile.getType() == GameLib::TileType::TOWN_HALL)
    {
      town_hall = &tile;
      revealAroundTile(tile.getIndex(), 0.5f, 2);
      break;
    }
  }
  return true;
}

void Map::generateMap()
{
  tiles.reserve(static_cast<unsigned long long>(num_tiles));
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  size_t index = 0;
  for (int i = 0; i < num_tiles; --num_tiles)
  {
    std::uniform_real_distribution<double> dist(0,
                                                static_cast<double>(num_tiles));
    auto randnum = static_cast<unsigned long long>(dist(gen));
    tiles.emplace_back(ServerTile(tiletypes.at(randnum), index));
    tiles.back().setSize(tile_size);
    tiletypes.erase(tiletypes.begin() + static_cast<long long>(randnum));
    index++;
  }
}

void Map::positionTiles()
{
  float xpos = 0;
  float ypos = 0;

  for (int i = 0; i < static_cast<int>(tiles.size()); i++)
  {
    tiles[i].setPosition(Point(xpos, ypos));

    xpos += tile_size;
    if (xpos >= tile_size * static_cast<float>(map_width))
    {
      xpos = 0;
      ypos += tile_size;
    }
  }
}

void Map::distributeTiles(json& json_file)
{
  GameLib::TileType type;
  int probability = 0;

  while (num_tiles < max_num_buildings)
  {
    // Loop through each building type once
    for (const json& building : json_file["building_number"])
    {
      type = static_cast<GameLib::TileType>(building["type"]);
      if (num_buildings[type] == building["max"])
      {
        continue;
      }

      // Using the probability field, roll a random number and check to see if
      // this should create a new tile of this type
      probability = building["probability"];
      if (probability >= probability_roll(generator))
      {
        num_buildings[type]++;
        num_tiles++;
      }
      if (num_tiles == max_num_buildings)
      {
        return;
      }
    }
  }
}

void Map::populateTileTypes()
{
  for (auto& [key, val] : num_buildings)
  {
    for (int i = 0; i < val; ++i)
    {
      tiletypes.push_back(key);
    }
  }
}

void Map::setMinResources(json& json_file)
{
  for (ServerTile& tile : tiles)
  {
    std::string building_string =
      json_file["id_to_name"][std::to_string(static_cast<int>(tile.getType()))];
    tile.getResources().water =
      json_file["buildings"][building_string]["water"]["min"];
    tile.getResources().food =
      json_file["buildings"][building_string]["food"]["min"];
    tile.getResources().wood =
      json_file["buildings"][building_string]["wood"]["min"];
    tile.getResources().metal =
      json_file["buildings"][building_string]["metal"]["min"];
    tile.getResources().med = json_file["buildings"][building_string]["medicin"
                                                                      "e"]["mi"
                                                                           "n"];
    tile.getResources().fuel =
      json_file["buildings"][building_string]["fuel"]["min"];
    resource_pool = resource_pool - tile.getResources();
    resource_pool_total -= tile.getResources().total();
  }
}

// Takes a json file that's the max min and probability.
// Then changes the passed resources taking from the passed resource pool
void Map::distributeResources(json& json_file, int& resource, int& pool)
{
  if (pool && json_file["probability"] >= probability_roll(generator))
  {
    resource++;
    pool--;
    resource_pool_total--;
  }
}

void Map::sendMap()
{
  GameLib::NetworkPacket packet(GameLib::EventType::MAP_DATA);
  packet << tiles.size() << tile_size;
  for (ServerTile tile : tiles)
  {
    packet << tile.getType() << tile.getPosition() << tile.getResources()
           << tile.getFade() << tile.isScouted() << tile.getHealth();
  }
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void Map::sendMapToID(int id)
{
  GameLib::NetworkPacket packet(GameLib::EventType::MAP_DATA);
  packet << tiles.size() << tile_size;
  for (ServerTile tile : tiles)
  {
    packet << tile.getType() << tile.getPosition() << tile.getResources()
           << tile.getFade() << tile.isScouted() << tile.getHealth();
  }
  Locator::instance->getNetworkManager()->sendDataToClient(packet, id);
}

void Map::setTileNeighbours()
{
  for (ServerTile& first : tiles)
  {
    for (ServerTile& second : tiles)
    {
      if (first.getPosition().distanceTo(second.getPosition()) < tile_size + 1)
      {
        first.addNeighbour(second.getIndex());
      }
    }
  }
}

void Map::revealAroundTile(size_t tile_index, float fade, int radius)
{
  for (size_t& index : tiles[tile_index].getNeighbours())
  {
    tiles[index].setFade(fade);
    GameLib::NetworkPacket packet(GameLib::EventType::REVEAL_TILE);
    packet << index << fade;
    Locator::instance->getNetworkManager()->sendDataToClients(packet);

    if (radius)
    {
      revealAroundTile(index, fade, radius - 1);
    }
  }
  tiles[tile_index].setFade(fade);
  GameLib::NetworkPacket packet(GameLib::EventType::REVEAL_TILE);
  packet << tile_index << fade;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void Map::damageTilesInLine(Point line_start,
                            Point line_end,
                            const float& damage,
                            const float& radius)
{
  // Calculate the distance to move each step
  Point current_pos = line_start;
  float radius_dist = radius * (tile_size / 2);
  float total_dist = line_start.distanceTo(line_end);
  if (total_dist == 0)
  {
    // Prevents divide by 0
    total_dist = 0.1f;
  }
  Point difference = line_end - line_start;
  Point step = difference / (total_dist / (radius_dist / 2));

  // Find the number of steps to reach the end point
  float length1 = difference.distanceTo(Point(0, 0));
  float length2 = step.distanceTo(Point(0, 0));
  float amount = 1;
  if (length2 > 0)
  {
    amount = length1 / length2;
  }

  int iterations = static_cast<int>(std::ceil(amount));
  if (iterations == 0)
  {
    iterations = 1;
  }

  // Loop once for each step
  for (int i = 0; i < iterations; i++)
  {
    // Loop through each tile, check to see if its in range then flag it
    // accordingly
    for (ServerTile& tile : tiles)
    {
      if (current_pos.distanceTo(tile.getPosition()) <= radius_dist)
      {
        tile.setHit(true);
      }
    }
    current_pos = current_pos + step;
  }

  // Loop through all tiles again applying damage and resetting the flag
  for (ServerTile& tile : tiles)
  {
    if (tile.getHit())
    {
      // Send out a packet to use internally in the server
      GameLib::NetworkPacket packet(GameLib::EventType::SET_TILE_HEALTH);
      packet << GameLib::CharacterType::NONE << tile.getIndex() << damage;
      notify(packet.getType(), packet);
      // The tile sends a packet to the clients to update health
      tile.damage(damage);
      tile.setHit(false);
    }
  }
}