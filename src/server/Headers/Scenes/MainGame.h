#ifndef PROJECT_MAINGAME_H
#define PROJECT_MAINGAME_H

#include "../Map.h"
#include "ServerScene.h"
#include "gamelib/Headers/NetworkPacket.h"
#include "server/Headers/Characters/CharacterManager.h"

class MainGame
  : public ServerScene
  , public GameLib::Observer
{
 public:
  ~MainGame() override;
  explicit MainGame(PersistentServerData& data);

  void load() override;
  GameLib::SceneType update() override;

  void clientReconnected(GameLib::NetworkPacket& data);
  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  bool game_ending = false;
  float time_elapsed = 0;

  Map main_map;
  CharacterManager character_manager;
  EventManager event_manager;
  int player_turn = -1; // id of the current players turn
};

#endif // PROJECT_MAINGAME_H
