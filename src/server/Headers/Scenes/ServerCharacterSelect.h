#ifndef PROJECT_SERVERCHARACTERSELECT_H
#define PROJECT_SERVERCHARACTERSELECT_H

#include "ServerScene.h"
#include "gamelib/Headers/Observer.h"

class ServerCharacterSelect
  : public ServerScene
  , public GameLib::Observer
{
 public:
  ServerCharacterSelect() = default;
  explicit ServerCharacterSelect(PersistentServerData& data) :
    ServerScene(data){};
  ~ServerCharacterSelect() override;

  void load() override;
  GameLib::SceneType update() override;

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  void selectCharacter(const GameLib::CharacterType& character,
                       GameLib::NetworkPacket& data);
  void clientReconnected(GameLib::NetworkPacket& data);
};

#endif // PROJECT_SERVERCHARACTERSELECT_H
