#include <client/Headers/UI/StockpileDisplay.h>

#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"

StockpileDisplay::StockpileDisplay() :
  stockpile_box(
    std::make_unique<AnimatedSprite>("data/ResourceBar.png", false, false)),
  click_area(ClickArea(Point(110, 20), 880, 50))
{
  stockpile_box->setWidth(880);
  stockpile_box->setHeight(50);
  stockpile_box->xPos(110);
  stockpile_box->yPos(20);

  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(160, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));
  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(304, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));
  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(448, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));
  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(592, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));
  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(736, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));
  stockpile_num.emplace_back(std::make_unique<TextBox>(
    Point(880, 24), "= ", 100, 30, 0.8, ASGE::COLOURS::WHITE));

  addControlButton(Point(130, 24),
                   Resources(1, 0, 0, 0, 0, 0),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(130, 44),
                   Resources(-1, 0, 0, 0, 0, 0),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");
  addControlButton(Point(255, 24),
                   Resources(0, 1, 0, 0, 0, 0),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(255, 44),
                   Resources(0, -1, 0, 0, 0, 0),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");
  addControlButton(Point(397, 24),
                   Resources(0, 0, 0, 0, 0, 1),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(397, 44),
                   Resources(0, 0, 0, 0, 0, -1),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");
  addControlButton(Point(537, 24),
                   Resources(0, 0, 1, 0, 0, 0),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(537, 44),
                   Resources(0, 0, -1, 0, 0, 0),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");
  addControlButton(Point(686, 24),
                   Resources(0, 0, 0, 1, 0, 0),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(686, 44),
                   Resources(0, 0, 0, -1, 0, 0),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");
  addControlButton(Point(831, 24),
                   Resources(0, 0, 0, 0, 1, 0),
                   "data/Buttons/Plus.png",
                   "data/Buttons/Plus2.png");
  addControlButton(Point(831, 44),
                   Resources(0, 0, 0, 0, -1, 0),
                   "data/Buttons/Minus.png",
                   "data/Buttons/Minus2.png");

  loaded = true;
}

void StockpileDisplay::addControlButton(const Point& pos,
                                        const Resources& resources,
                                        const std::string& file_path_one,
                                        const std::string& file_path_two)
{
  resources_controllers.push_back(
    std::make_unique<Button>(pos,
                             file_path_one,
                             file_path_two,
                             GameLib::EventType::INCREMENT_A_RESOURCES,
                             20,
                             20));
  resources_controllers.back()->addObserver(
    this, GameLib::EventType::INCREMENT_A_RESOURCES);
  resources_controllers.back()->sendResources(true);
  resources_controllers.back()->setResources(resources);
}

void StockpileDisplay::render(double delta_time)
{
  if (loaded)
  {
    stockpile_box->render(delta_time);
    for (int i = 0;
         static_cast<unsigned long long>(i) < resources_controllers.size();
         i++)
    {
      resources_controllers[i]->render(delta_time);
    }

    for (int i = 0; static_cast<unsigned long long>(i) < stockpile_num.size();
         i++)
    {
      stockpile_num[i]->render(delta_time);
    }
  }
}

void StockpileDisplay::updateButtons()
{
  bool show_buttons = local_character->getTile()->getName() == "Town Hall";

  for (auto& button : resources_controllers)
  {
    button->setVisible(show_buttons);
    button->setEnabled(show_buttons);
  }
}

void StockpileDisplay::changeInfo(Character* character)
{
  local_character = character;
  if (!character || !map->getTownHall())
  {
    return;
  }

  stockpile_num[0]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().water) + "/" +
    std::to_string(win_display.water));
  stockpile_num[1]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().food) + "/" +
    std::to_string(win_display.food));
  stockpile_num[2]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().med) + "/" +
    std::to_string(win_display.med));
  stockpile_num[3]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().wood) + "/" +
    std::to_string(win_display.wood));
  stockpile_num[4]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().metal) + "/" +
    std::to_string(win_display.metal));
  stockpile_num[5]->setText(
    "= " + std::to_string(map->getTownHall()->getResources().fuel) + "/" +
    std::to_string(win_display.fuel));

  updateButtons();
}

void StockpileDisplay::setMap(Map* new_map)
{
  map = new_map;
}

void StockpileDisplay::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::INCREMENT_A_RESOURCES)
  {
    Resources res;
    data >> res;

    GameLib::NetworkPacket packet(GameLib::EventType::DEPOSIT_RESOURCES);
    packet << local_character->getCharacterType() << res;
    Locator::instance->getNetworkManager()->sendDataToServer(packet);
  }
}

void StockpileDisplay::setWinDisplay(const Resources& win_amount)
{
  win_display = win_amount;
}
