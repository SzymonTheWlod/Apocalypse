#include <Engine/Renderer.h>

#include "client/Headers/Tile.h"
#include "client/Headers/UI/CharInfoDisplay.h"
#include "client/Headers/UI/InfoDisplay.h"
#include "client/Headers/UI/StockpileDisplay.h"

#ifndef PROJECT_UIMANAGER_H
#  define PROJECT_UIMANAGER_H

class UIManger : public GameLib::Observer
{
 public:
  UIManger();

  void render(double delta_time);

  void displayTileInformation(Tile* clicked_tile);
  void displayCharInformation(Character* clicked_char);
  void displayStockpileInfo();

  void showCharInfo(bool display);
  void showTileInfo(bool display);
  void setCharManager(CharacterManager* manager)
  {
    character_manager = manager;
  };

  void setWinAmount(Resources win_amount);

  void setMap(Map* map);
  bool doesClickHitUI(const Point& point);

  void onNotify(GameLib::NetworkPacket& data);

  bool closeWindows();

 private:
  std::unique_ptr<InfoDisplay> tile_info = nullptr;
  std::unique_ptr<CharInfoDisplay> char_display = nullptr;
  std::unique_ptr<TextBox> action_points = nullptr;
  std::unique_ptr<Button> end_turn = nullptr;
  std::unique_ptr<StockpileDisplay> stockpile = nullptr;

  CharacterManager* character_manager = nullptr;
};

#endif // PROJECT_UIMANAGER_H
