#include "client/Headers/AudioManager.h"
#include "client/Headers/FileReader.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
#include <Engine/Input.h>
#include <client/Headers/Map.h>

Map::Map()
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::MAP_DATA);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::REVEAL_TILE);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_SCOUTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_TILE_RESOURCES);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_TILE_HEALTH);
}

Map::~Map()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::MAP_DATA);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::REVEAL_TILE);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_SCOUTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_TILE_RESOURCES);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_TILE_HEALTH);
}

bool Map::load(GameLib::NetworkPacket& data)
{
  nlohmann::json config;
  FileReader().readJsonFile("data/config/TileData.json", config);
  float size;
  Point smallest = Point(0, 0);
  Point largest = Point(0, 0);
  data >> num_tiles >> size;
  for (unsigned long long i = 0; i < num_tiles; i++)
  {
    GameLib::TileType tiletype;
    Point position;
    Resources resources;
    float fade;
    bool scouted;
    float health;
    data >> tiletype >> position >> resources >> fade >> scouted >> health;

    tiles.push_back(std::make_unique<Tile>(tiletype, size, i, config));

    tiles.back()->setPosition(position);
    tiles.back()->setResources(resources);
    tiles.back()->setFade(fade);
    tiles.back()->setScouted(scouted);
    tiles.back()->setHealth(health);

    smallest.x = position.x < smallest.x ? position.x : smallest.x;
    smallest.y = position.y < smallest.y ? position.y : smallest.y;
    largest.x = position.x > largest.x ? position.x : largest.x;
    largest.y = position.y > largest.y ? position.y : largest.y;
  }

  largest.x /= 1.4f;
  largest.y /= 1.4f;
  Locator::instance->getRenderer()->setCameraLimits(smallest - Point(400, 400),
                                                    largest);

  for (auto& tile : tiles)
  {
    if (tile->getType() == GameLib::TileType::TOWN_HALL)
    {
      town_hall = tile.get();
      break;
    }
  }
  loaded = true;
  return true;
}

void Map::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType::MAP_DATA:
    {
      load(data);
      break;
    }
    case GameLib::EventType::REVEAL_TILE:
    {
      size_t index;
      float fade;
      data >> index >> fade;
      tiles[index]->setFade(fade);
      break;
    }
    case GameLib::EventType::SET_SCOUTED:
    {
      Locator::instance->getAudioManager()->playSoundFX(GameLib::SoundFX::SCOUT,
                                                        0.5f);
      size_t index;
      data >> index;
      tiles[index]->setScouted(true);
      break;
    }
    case GameLib::EventType::SET_TILE_RESOURCES:
    {
      Locator::instance->getAudioManager()->playSoundFX(
        GameLib::SoundFX::SCAVANGE);
      size_t index;
      Resources res;
      data >> index >> res;
      tiles[index]->setResources(res);
      break;
    }
    case GameLib::EventType ::SET_TILE_HEALTH:
    {
      size_t index;
      float health;
      data >> index >> health;
      tiles[index]->setHealth(health);
      break;
    }
    default:
    {
      break;
    }
  }
}

void Map::render(double delta_time)
{
  if (loaded)
  {
    for (int i = 0; i < static_cast<int>(num_tiles); i++)
    {
      tiles[i]->render(delta_time);
    }
  }
}

Tile* Map::checkIfClicked(Point click_pos)
{
  for (int i = 0; i < static_cast<int>(num_tiles); i++)
  {
    if (tiles[i]->checkForClick(click_pos))
    {
      return tiles[i].get();
    }
  }
  return nullptr;
}

void Map::update()
{
  Point mouse = Locator::instance->getInputManager()->getWorldMousePosition();
  for (auto& tile : tiles)
  {
    tile->setHighlighted(tile->checkForClick(mouse));
  }
}