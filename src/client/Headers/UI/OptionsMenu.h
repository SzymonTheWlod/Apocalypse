#ifndef PROJECT_OPTIONSMENU_H
#define PROJECT_OPTIONSMENU_H

#include "client/Headers/UI/Button.h"
#include "client/Headers/UI/Image.h"
#include "client/Headers/UI/ProgressBar.h"

class OptionsMenu : public GameLib::Observer
{
 public:
  OptionsMenu();
  ~OptionsMenu() override = default;

  void render(double delta_time);

  void onNotify(GameLib::NetworkPacket& data) override;

  void show();
  void hide();
  bool visible() { return active; };

  void useExitButtons(bool flag) { use_exit_buttons = flag; };
  void addSceneChangeObserver(GameLib::Observer* observer);

 private:
  void updateModeText();

  std::unique_ptr<AnimatedSprite> background;

  std::unique_ptr<ProgressBar> music_volume;
  std::unique_ptr<Button> music_volume_down;
  std::unique_ptr<Button> music_volume_up;

  std::unique_ptr<ProgressBar> fx_volume;
  std::unique_ptr<Button> fx_volume_down;
  std::unique_ptr<Button> fx_volume_up;

  std::unique_ptr<Button> window_mode;

  std::unique_ptr<Button> close;

  std::unique_ptr<Button> exit_menu;
  std::unique_ptr<Button> exit_game;

  Point pos = Point(420, 120);
  float width = 420;
  float height = 483;
  ClickArea click_area = ClickArea(pos, width, height);

  bool active = false;
  bool use_exit_buttons = false;
};

#endif // PROJECT_OPTIONSMENU_H
