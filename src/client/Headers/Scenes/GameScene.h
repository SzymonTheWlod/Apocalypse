#ifndef PROJECT_GAMESCENE_H
#define PROJECT_GAMESCENE_H

#include "../Map.h"
#include "../Network/ClientConnection.h"
#include "../Tile.h"
#include "Scene.h"
#include "client/Headers/Characters/CharacterManager.h"
#include "client/Headers/Characters/ClientScout.h"
#include "client/Headers/ClientEvent.h"
#include "client/Headers/UI/ChatWindow.h"
#include "client/Headers/UI/Image.h"
#include "client/Headers/UI/OptionsMenu.h"
#include "client/Headers/UI/TextInput.h"
#include "client/Headers/UI/UIManager.h"
#include "gamelib/Headers/Constants.h"

class GameScene
  : public Scene
  , public GameLib::Observer
{
 public:
  GameScene() = default;
  explicit GameScene(PersistentClientData& data) : Scene(data){};
  ~GameScene() override;
  bool load() override;
  GameLib::SceneType update(double delta_time) override;
  void render(double delta_time) override;
  void keyHandler(ASGE::SharedEventData data) override;
  void mouseHandler(ASGE::ClickEvent& click) override;

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  void loadAllSounds();

  void chatEvents(GameLib::NetworkPacket& data);
  void cameraControls(const ASGE::KeyEvent* key);

  std::unique_ptr<ChatWindow> chat_window = nullptr;
  std::unique_ptr<TextInput> chat_input = nullptr;
  std::unique_ptr<Image> chat_title = nullptr;
  std::unique_ptr<Button> hide_chat = nullptr;
  std::unique_ptr<Button> show_chat = nullptr;

  std::unique_ptr<AnimatedSprite> disconnected_overlay = nullptr;
  std::unique_ptr<AnimatedSprite> lose_screen = nullptr;
  std::unique_ptr<AnimatedSprite> win_screen = nullptr;

  bool won_game = false;
  bool lost_game = false;

  std::unique_ptr<Map> map = nullptr;
  ClientEvent clientEvent;

  OptionsMenu options_menu;
  UIManger ui_manager;
  CharacterManager character_manager;

  Point camera_move = Point(0, 0);

  bool waiting_for_reconnect = false;
};

#endif // PROJECT_GAMESCENE_H
