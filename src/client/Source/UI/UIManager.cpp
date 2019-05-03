
#include <client/Headers/UI/UIManager.h>

#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
#include "client/Headers/UI/UIManager.h"

UIManger::UIManger() :
  tile_info(std::make_unique<InfoDisplay>()),
  char_display(std::make_unique<CharInfoDisplay>()),
  action_points(std::make_unique<TextBox>(
    Point(1000, 15), "0", 270, 100, 1.5f, ASGE::COLOURS::WHITE)),
  end_turn(std::make_unique<Button>(Point(1090, 28),
                                    "data/Buttons/Button_End.png",
                                    "data/Buttons/Button_End2.png",
                                    GameLib::EventType::END_TURN,
                                    132,
                                    33)),
  stockpile(std::make_unique<StockpileDisplay>())
{
  end_turn->addObserver(this, GameLib::EventType::END_TURN);
  action_points->setBackgroundSprite("data/action_points.png");
}

void UIManger::render(double delta_time)
{
  tile_info->render(delta_time);
  char_display->render(delta_time);
  stockpile->render(delta_time);

  if (character_manager->getLocalCharacter())
  {
    action_points->setText(std::to_string(
      character_manager->getLocalCharacter()->getActionPoints()));
  }
  action_points->render(delta_time);
  end_turn->render(delta_time);
}

void UIManger::displayTileInformation(Tile* clicked_tile)
{
  tile_info->changeInfo(clicked_tile, character_manager->getLocalCharacter());
}

void UIManger::displayCharInformation(Character* clicked_char)
{
  char_display->setClickedCharacter(clicked_char);
  char_display->setLocalCharacter(character_manager->getLocalCharacter());
}

void UIManger::displayStockpileInfo()
{
  stockpile->changeInfo(character_manager->getLocalCharacter());
}

void UIManger::showTileInfo(bool display)
{
  tile_info->setVisible(display);
}

void UIManger::showCharInfo(bool display)
{
  char_display->setVisible(display);
}

bool UIManger::doesClickHitUI(const Point& point)
{
  return (tile_info->isVisible() && tile_info->isPointInArea(point)) ||
         (stockpile->isVisible() && stockpile->isPointInArea(point)) ||
         (char_display->isVisible() && char_display->isPointInArea(point)) ||
         end_turn->doesClickHit(point);
}

void UIManger::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::END_TURN)
  {
    character_manager->getLocalCharacter()->setActionPoints(0);
    data << character_manager->getLocalCharacter()->getCharacterType();
    Locator::instance->getNetworkManager()->sendDataToServer(data);
  }
}

void UIManger::setMap(Map* map)
{
  stockpile->setMap(map);
}

void UIManger::setWinAmount(Resources win_amount)
{
  stockpile->setWinDisplay(win_amount);
}

bool UIManger::closeWindows()
{
  if (tile_info->isVisible())
  {
    tile_info->setVisible(false);
    return true;
  }
  if (char_display->isVisible())
  {
    char_display->setVisible(false);
    return true;
  }
  return false;
}
