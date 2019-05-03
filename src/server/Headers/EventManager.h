#ifndef PROJECT_EVENTS_H
#define PROJECT_EVENTS_H

#include <iostream>
#include <json.hpp>
#include <random>
#include <server/Headers/Map.h>

#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/NetworkPacket.h"

using json = nlohmann::json;

class EventManager
{
 public:
  EventManager();
  //   ~Events() override = default;
  void disasterBar();
  void event();
  void turnEnded();
  void setMap(Map* new_map) { map = new_map; }

  void syncData();

 private:
  std::mt19937 gen;
  std::uniform_int_distribution<int> probability_roll;

  Map* map = nullptr;
  double event_val = 0;
  double apocalypse_val = 0;
  double apocalypse_total = 0;

  GameLib::DisasterType type = GameLib::DisasterType::BOTTOM_ROW;
  int randnum = 0;
  bool event_found = false;
};

#endif // PROJECT_EVENTS_H
