#ifndef PROJECT_INFODISPLAY_H
#define PROJECT_INFODISPLAY_H

#include <Engine/Renderer.h>

#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/Characters/CharacterManager.h"
#include "client/Headers/Tile.h"
#include "client/Headers/UI/Button.h"
#include "client/Headers/UI/ProgressBar.h"
#include "client/Headers/UI/TextBox.h"

class InfoDisplay : public GameLib::Observer
{
 public:
  InfoDisplay();

  void render(double delta_time);

  void changeInfo(Tile* tile_clicked, Character* character);

  void setVisible(bool show);
  bool isVisible() { return visible; };

  void onNotify(GameLib::NetworkPacket& data) override;

  bool isPointInArea(const Point& point)
  {
    return click_area.isPointInArea(point);
  };

 private:
  void updateButtons();

  ClickArea click_area;

  std::vector<std::unique_ptr<TextBox>> resources_num;
  std::unique_ptr<TextBox> building_name = nullptr;

  std::unique_ptr<AnimatedSprite> background_box = nullptr;
  std::unique_ptr<ProgressBar> building_healthbar = nullptr;

  std::unique_ptr<Button> move;
  std::unique_ptr<Button> close;
  std::unique_ptr<Button> scout;
  std::unique_ptr<Button> scavenge;
  std::unique_ptr<Button> build_bunker;
  std::unique_ptr<Button> deposit_all;

  Character* local_character = nullptr;
  Tile* displayed_tile = nullptr;

  bool loaded = false;
  bool visible = false;
};

#endif // PROJECT_INFODISPLAY_H
