
#include "server/Headers/EventManager.h"
#include "gamelib/Headers/Constants.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Network/NetworkManager.h"
#include <fstream>
#include <server/Headers/EventManager.h>

EventManager::EventManager()
{
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  gen = std::mt19937(seed);
  probability_roll = std::uniform_int_distribution<int>(1, 100);
}

void EventManager::disasterBar()
{
  GameLib::NetworkPacket packet(GameLib::EventType::DISASTER_BAR);
  packet << apocalypse_val;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}

void EventManager::turnEnded()
{
  event();
  apocalypse_val += 0.0004;
  event_val += 0.1;
  event_val += apocalypse_val * 25;
  apocalypse_total += apocalypse_val;
  disasterBar();
}

void EventManager::event()
{ // Events
  if (event_val < 1)
  {
    return;
  }
  json json_file;
  std::ifstream stream;
  stream.open("server/EventInfo.json");
  stream >> json_file;

  Point start_pos;
  Point end_pos;
  int event_id = 0;
  while (!event_found)
  {
    int probability = 0;
    randnum = probability_roll(gen);
    for (const json& event : json_file["event_number"])
    {
      probability += static_cast<int>(event["probability"]);
      if (probability >= randnum)
      {
        // Getting all the Different Data
        type = static_cast<GameLib::DisasterType>(event["type"]);
        float damage = event["damage"];
        float radius = event["radius"];
        event_id = event["id"];
        event_found = true;

        switch (type)
        {
          case GameLib::DisasterType::BOTTOM_ROW:
          {
            start_pos = map->getTile(map->getWidth() * (map->getHeight() - 1))
                          .getPosition();
            end_pos = map->getTile((map->getWidth() * map->getHeight()) - 1)
                        .getPosition();
            map->damageTilesInLine(start_pos, end_pos, damage, radius);
            end_pos = start_pos;
            start_pos = end_pos + Point(0, 50);
            end_pos = end_pos - Point(0, 0);
            break;
          }
          case GameLib::DisasterType::TOP_ROW:
          {
            start_pos = map->getTile(0).getPosition();
            end_pos = map->getTile(map->getWidth() - 1).getPosition();
            map->damageTilesInLine(start_pos, end_pos, damage, radius);
            end_pos = start_pos;
            start_pos = end_pos - Point(0, 50);
            break;
          }
          case GameLib::DisasterType::RAND_COLUMN:
          {
            std::uniform_int_distribution<size_t> coloumn_dist =
              std::uniform_int_distribution<size_t>(0, map->getWidth() - 1);
            size_t index_column = coloumn_dist(gen);
            start_pos = map->getTile(index_column).getPosition();
            end_pos = map
                        ->getTile(((map->getHeight() - 1) * map->getWidth()) +
                                  index_column)
                        .getPosition();
            map->damageTilesInLine(start_pos, end_pos, damage, radius);
            break;
          }
          case GameLib::DisasterType::RAND_TILE:
          {
            std::uniform_int_distribution<size_t> tile_dist =
              std::uniform_int_distribution<size_t>(0,
                                                    map->getMap().size() - 1);
            size_t index = tile_dist(gen);
            start_pos = map->getTile(index).getPosition() +
                        Point((radius - 1) * 25, (radius - 1) * 25);
            end_pos = map->getTile(index).getPosition() +
                      Point((radius - 1) * 25, (radius - 1) * 25);
            map->damageTilesInLine(start_pos, end_pos, damage, radius);
            break;
          }
        }
        break;
      }
    }
  }
  event_found = false;
  GameLib::NetworkPacket EventPacket(GameLib::EventType::EVENTS);
  EventPacket << event_id << start_pos << end_pos;
  event_val = 0;
  Locator::instance->getNetworkManager()->sendDataToClients(EventPacket);
}

void EventManager::syncData()
{
  GameLib::NetworkPacket packet(GameLib::EventType::DISASTER_BAR);
  packet << apocalypse_total;
  Locator::instance->getNetworkManager()->sendDataToClients(packet);
}