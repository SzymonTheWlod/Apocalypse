#ifndef PROJECT_CHARINFODISPLAY_H
#define PROJECT_CHARINFODISPLAY_H

#include <Engine/Renderer.h>

#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/Characters/CharacterManager.h"
#include "client/Headers/Tile.h"
#include "client/Headers/UI/Button.h"
#include "client/Headers/UI/ProgressBar.h"
#include "client/Headers/UI/TextBox.h"

class CharInfoDisplay : public GameLib::Observer
{
 public:
  CharInfoDisplay();

  void render(double delta_time);
  void changeInfo();

  void setClickedCharacter(Character* char_clicked);

  void setVisible(bool show);
  bool isVisible() { return visible; };

  void onNotify(GameLib::NetworkPacket& data) override;

  bool isPointInArea(const Point& point)
  {
    return click_area.isPointInArea(point);
  };

  void setLocalCharacter(Character* local) { local_character = local; };

 private:
  ClickArea click_area;

  std::vector<std::unique_ptr<TextBox>> resources_num;
  std::unique_ptr<TextBox> char_name = nullptr;

  std::unique_ptr<Button> close = nullptr;
  std::unique_ptr<Button> heal_character = nullptr;

  std::unique_ptr<AnimatedSprite> char_portrait = nullptr;
  std::unique_ptr<AnimatedSprite> background_box = nullptr;
  std::unique_ptr<ProgressBar> char_healthbar = nullptr;

  Character* character = nullptr;
  Character* local_character = nullptr;

  bool visible = false;
  bool loaded = false;

  GameLib::CharacterType current_char = GameLib::CharacterType ::NONE;
};

#endif // PROJECT_CHARINFODISPLAY_H
