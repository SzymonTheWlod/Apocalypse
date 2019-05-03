#ifndef PROJECT_CHARACTERSELECT_H
#define PROJECT_CHARACTERSELECT_H

#include "Scene.h"
#include "client/Headers/UI/Button.h"
#include "client/Headers/UI/TextBox.h"
#include "gamelib/Headers/Observer.h"

class CharacterSelect
  : public Scene
  , public GameLib::Observer
{
 public:
  CharacterSelect() = default;
  explicit CharacterSelect(PersistentClientData& data) : Scene(data){};
  ~CharacterSelect() override;

  bool load() override;
  GameLib::SceneType update(double delta_time) override;
  void render(double delta_time) override;
  void keyHandler(ASGE::SharedEventData data) override;
  void mouseHandler(ASGE::ClickEvent& click) override;

  void onNotify(GameLib::NetworkPacket& data) override;
  void removeCharacter(GameLib::CharacterType& type,
                       const std::string& replacement_texture);

 private:
  void loadButtons();

  std::unique_ptr<TextBox> select_char = nullptr;
  std::unique_ptr<AnimatedSprite> background = nullptr;
  std::unique_ptr<AnimatedSprite> disconnected_overlay = nullptr;

  std::unique_ptr<TextBox> scout_info = nullptr;
  std::unique_ptr<TextBox> medic_info = nullptr;
  std::unique_ptr<TextBox> builder_info = nullptr;
  std::unique_ptr<TextBox> soldier_info = nullptr;

  std::unique_ptr<UI> scout = nullptr;
  std::unique_ptr<UI> medic = nullptr;
  std::unique_ptr<UI> builder = nullptr;
  std::unique_ptr<UI> soldier = nullptr;

  bool waiting_for_reconnect = false;
};

#endif // PROJECT_CHARACTERSELECT_H
