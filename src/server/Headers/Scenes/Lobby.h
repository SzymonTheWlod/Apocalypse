#ifndef PROJECT_LOBBY_H
#define PROJECT_LOBBY_H

#include "ServerScene.h"
#include "gamelib/Headers/Observer.h"
#include "gamelib/Headers/Printer.h"

class Lobby
  : public ServerScene
  , public GameLib::Observer
{
 public:
  Lobby() = default;
  ~Lobby() override;

  void load() override;
  GameLib::SceneType update() override;

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
};

#endif // PROJECT_LOBBY_H
