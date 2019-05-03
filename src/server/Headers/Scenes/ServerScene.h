#ifndef PROJECT_SERVERSCENE_H
#define PROJECT_SERVERSCENE_H

#include "PersistentServerData.h"
#include "gamelib/Headers/Constants.h"

class ServerScene
{
 public:
  ServerScene() = default;
  explicit ServerScene(PersistentServerData& data) : server_data(data){};
  virtual ~ServerScene() = default;
  virtual void load() = 0;
  virtual GameLib::SceneType update() = 0;

  PersistentServerData& getPersistentData() { return server_data; };

 protected:
  PersistentServerData server_data;
  GameLib::SceneType next_scene = GameLib::SceneType::NO_CHANGE;
};

#endif // PROJECT_SERVERSCENE_H
