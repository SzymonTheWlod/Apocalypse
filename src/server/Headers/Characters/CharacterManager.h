#ifndef PROJECT_CHARACTERMANAGER_H
#define PROJECT_CHARACTERMANAGER_H

#include "ServerBuilder.h"
#include "ServerMedic.h"
#include "ServerScout.h"
#include "ServerSoldier.h"
#include "gamelib/Headers/Observer.h"
#include <bits/unique_ptr.h>
#include <map>
#include <server/Headers/EventManager.h>

class CharacterManager
  : public GameLib::Observer
  , public GameLib::Subject
{
 public:
  CharacterManager();
  ~CharacterManager() override;

  void addCharacter(GameLib::CharacterType character);
  void calculateWinCondition();

  void nextTurn(GameLib::CharacterType character);

  void onNotify(GameLib::NetworkPacket& data) override;
  void tileEvents(GameLib::NetworkPacket& data);
  void syncAllData(int id);

  void setMap(Map* new_map);
  void setEventManager(EventManager* event) { event_manager = event; };

 private:
  void checkForWin();
  void damagePlayersOnTile(GameLib::NetworkPacket& data);

  void updateFogOfWar(const size_t& start_tile,
                      const GameLib::CharacterType& character);

  std::map<GameLib::CharacterType, Character> characters;

  Map* map = nullptr;
  EventManager* event_manager;
  Resources win_amount;

  GameLib::CharacterType players_turn = GameLib::CharacterType ::MEDIC;
};

#endif // PROJECT_CHARACTERMANAGER_H
