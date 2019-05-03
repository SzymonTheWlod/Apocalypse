#ifndef PROJECT_STOCKPILEDISPLAY_H
#define PROJECT_STOCKPILEDISPLAY_H

#include <Engine/Renderer.h>
#include <client/Headers/ClientEvent.h>

#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/Characters/CharacterManager.h"
#include "client/Headers/UI/Button.h"
#include "client/Headers/UI/TextBox.h"
#include "gamelib/Headers/Observer.h"

class StockpileDisplay : public GameLib::Observer
{
 public:
  StockpileDisplay();

  void render(double delta_time);
  void changeInfo(Character* character);
  void updateButtons();

  void setMap(Map* new_map);
  void setWinDisplay(const Resources& win_amount);

  bool isVisible() { return visible; };

  bool isPointInArea(const Point& point)
  {
    return click_area.isPointInArea(point);
  };

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  void addControlButton(const Point& pos,
                        const Resources& resources,
                        const std::string& file_path_one,
                        const std::string& file_path_two);

  std::vector<std::unique_ptr<TextBox>> stockpile_num;
  std::vector<std::unique_ptr<Button>> resources_controllers;

  std::unique_ptr<AnimatedSprite> stockpile_box = nullptr;
  Map* map = nullptr;

  ClickArea click_area;

  Character* local_character = nullptr;
  Resources win_display;
  bool visible = false;
  bool loaded = false;
};

#endif // PROJECT_STOCKPILEDISPLAY_H
