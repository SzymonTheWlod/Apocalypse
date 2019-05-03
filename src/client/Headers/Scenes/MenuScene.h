#ifndef PROJECT_MENUSCENE_H
#define PROJECT_MENUSCENE_H

#include "Scene.h"
#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/UI/ChatWindow.h"
#include "client/Headers/UI/OptionsMenu.h"
#include "client/Headers/UI/TextInput.h"

#include <iostream>

namespace ASGE
{
class Renderer;
}
class AnimatedSprite;
/**
**   The main menu
*/
class MenuScene
  : public Scene
  , public GameLib::Observer
{
 public:
  MenuScene() = default;
  explicit MenuScene(PersistentClientData& data) : Scene(data){};
  ~MenuScene() override;
  bool load() override;
  GameLib::SceneType update(double delta_time) override;
  void render(double delta_time) override;
  void keyHandler(ASGE::SharedEventData data) override;
  void mouseHandler(ASGE::ClickEvent& click) override;

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  void connectionPackets(GameLib::NetworkPacket& data);

  bool ready = false;

  OptionsMenu options_menu;

  std::unique_ptr<AnimatedSprite> background = nullptr;
  std::unique_ptr<AnimatedSprite> instructions_panel = nullptr;
  bool instructions_open = false;

  std::unique_ptr<TextInput> username = nullptr;

  std::unique_ptr<TextInput> ip_input = nullptr;
  std::unique_ptr<TextBox> is_connected = nullptr;

  std::unique_ptr<Button> ready_up = nullptr;
  std::unique_ptr<TextBox> is_ready = nullptr;
  std::unique_ptr<TextBox> players_ready = nullptr;

  std::unique_ptr<TextInput> chat_input = nullptr;
  std::unique_ptr<ChatWindow> chat_window = nullptr;

  std::unique_ptr<Button> open_options = nullptr;
  std::unique_ptr<Button> exit_game = nullptr;
  std::unique_ptr<Button> instructions = nullptr;
};

#endif // PROJECT_MENUSCENE_H
