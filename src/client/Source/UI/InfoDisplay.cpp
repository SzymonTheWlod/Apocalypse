#include <client/Headers/UI/InfoDisplay.h>

#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"

InfoDisplay::InfoDisplay() :
  click_area(ClickArea(Point(10, 550), 540, 150)),
  background_box(
    std::make_unique<AnimatedSprite>("data/PopUpWindow.png", false, false))
{
  background_box->setWidth(440);
  background_box->setHeight(150);
  background_box->xPos(10);
  background_box->yPos(550);

  move = std::make_unique<Button>(Point(450, 557),
                                  "data/Buttons/Button_Move.png",
                                  "data/Buttons/Button_Move2.png",
                                  GameLib::EventType::MOVE_CHARACTER,
                                  100,
                                  25);
  move->addObserver(this, GameLib::EventType::MOVE_CHARACTER);
  scout = std::make_unique<Button>(Point(450, 587),
                                   "data/Buttons/Button_Scout.png",
                                   "data/Buttons/Button_Scout2.png",
                                   GameLib::EventType::SCOUT_TILE,
                                   100,
                                   25);
  scout->addObserver(this, GameLib::EventType::SCOUT_TILE);
  close = std::make_unique<Button>(Point(417, 558),
                                   "data/Buttons/close.png",
                                   "data/Buttons/close2.png",
                                   GameLib::EventType::CLOSE_TILEINFO,
                                   25,
                                   25);
  close->addObserver(this, GameLib::EventType::CLOSE_TILEINFO);
  scavenge = std::make_unique<Button>(Point(450, 617),
                                      "data/Buttons/Button_Scavenge.png",
                                      "data/Buttons/Button_Scavenge2.png",
                                      GameLib::EventType::SCAVENGE_TILE,
                                      100,
                                      25);
  scavenge->addObserver(this, GameLib::EventType::SCAVENGE_TILE);

  build_bunker = std::make_unique<Button>(Point(450, 647),
                                          "data/Buttons/Bunker.png",
                                          "data/Buttons/Bunker2.png",
                                          GameLib::EventType::BUILD_BUNKER,
                                          100,
                                          25);
  build_bunker->addObserver(this, GameLib::EventType::BUILD_BUNKER);
  deposit_all = std::make_unique<Button>(Point(450, 677),
                                         "data/Buttons/deposit.png",
                                         "data/Buttons/deposit2.png",
                                         GameLib::EventType::DEPOSIT_RESOURCES,
                                         100,
                                         25);
  deposit_all->addObserver(this, GameLib::EventType::DEPOSIT_RESOURCES);
  deposit_all->setEnabled(false);
  move->setEnabled(false);
  scavenge->setEnabled(false);
  scout->setEnabled(false);
  build_bunker->setEnabled(false);

  building_name = std::make_unique<TextBox>(
    Point(27, 554), "Name", 160, 30, 0.9f, ASGE::COLOURS::WHITE);

  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(70, 598), "", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(70, 644), "", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(205, 598), "", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(205, 644), "", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(345, 598), "", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(345, 644), "", 100, 30, 1, ASGE::COLOURS::WHITE));

  building_healthbar = std::make_unique<ProgressBar>(
    Point(215, 573), 200, 10, GameLib::Rotation::LEFT_TO_RIGHT);
  building_healthbar->addBackgroundSprite("data/red.png");
  building_healthbar->addFillSprite("data/green.png");
  loaded = true;
}

void InfoDisplay::render(double delta_time)
{
  if (loaded && visible)
  {
    updateButtons();
    changeInfo(displayed_tile, local_character);
    background_box->render(delta_time);
    building_healthbar->render(delta_time);
    building_name->render(delta_time);

    for (int i = 0; static_cast<unsigned long long>(i) < resources_num.size();
         i++)
    {
      resources_num[i]->render(delta_time);
    }

    move->render(delta_time);
    close->render(delta_time);
    scout->render(delta_time);
    scavenge->render(delta_time);
    build_bunker->render(delta_time);
    deposit_all->render(delta_time);
  }
}

void InfoDisplay::changeInfo(Tile* tile_clicked, Character* character)
{
  if (displayed_tile)
  {
    displayed_tile->setHighlightLock(false);
    displayed_tile->setHighlighted(false);
  }

  displayed_tile = tile_clicked;
  local_character = character;

  displayed_tile->setHighlighted(true);
  displayed_tile->setHighlightLock(true);

  if (tile_clicked->isRevealed() && tile_clicked->isScouted())
  {
    building_name->setText(tile_clicked->getName());
    building_healthbar->setProgress(tile_clicked->getHealthPercent());

    resources_num[0]->setText(
      "= " + std::to_string(tile_clicked->getResources().water));
    resources_num[1]->setText(
      "= " + std::to_string(tile_clicked->getResources().wood));
    resources_num[2]->setText(
      "= " + std::to_string(tile_clicked->getResources().food));
    resources_num[3]->setText(
      "= " + std::to_string(tile_clicked->getResources().metal));
    resources_num[4]->setText("= " +
                              std::to_string(tile_clicked->getResources().med));
    resources_num[5]->setText(
      "= " + std::to_string(tile_clicked->getResources().fuel));
  }
  else
  {
    if (tile_clicked->isRevealed())
    {
      building_name->setText(tile_clicked->getName());
      building_healthbar->setProgress(tile_clicked->getHealthPercent());
    }
    else
    {
      building_name->setText("????");
      building_healthbar->addProgress(0);
    }

    resources_num[0]->setText("?");
    resources_num[1]->setText("?");
    resources_num[2]->setText("?");
    resources_num[3]->setText("?");
    resources_num[4]->setText("?");
    resources_num[5]->setText("?");
  }
}

void InfoDisplay::setVisible(bool show)
{
  visible = show;
  move->setEnabled(show);
  close->setEnabled(show);
  scout->setEnabled(show);
  scavenge->setEnabled(show);

  if (!show && displayed_tile)
  {
    displayed_tile->setHighlightLock(false);
    displayed_tile->setHighlighted(true);
  }
}

void InfoDisplay::updateButtons()
{
  bool active;
  if (local_character->getActionPoints() == 0 || !displayed_tile->isRevealed())
  {
    active = false;
    move->setEnabled(false);
    move->setVisible(false);
  }
  else
  {
    active = displayed_tile == local_character->getTile();
    move->setEnabled(true);
    move->setVisible(true);
  }

  scout->setEnabled(active);
  scout->setVisible(active);
  scavenge->setEnabled(active);
  scavenge->setVisible(active);

  if (displayed_tile->isScouted())
  {
    scout->setEnabled(false);
    scout->setVisible(false);
  }
  if (displayed_tile->getResources().total() == 0)
  {
    scavenge->setEnabled(false);
    scavenge->setVisible(false);
  }

  if (displayed_tile->getName() == "Town Hall" &&
      displayed_tile == local_character->getTile())
  {
    scout->setEnabled(false);
    scout->setVisible(false);
    scavenge->setEnabled(false);
    scavenge->setVisible(false);
    build_bunker->setEnabled(true);
    build_bunker->setVisible(true);
    deposit_all->setEnabled(true);
    deposit_all->setVisible(true);
  }
  else
  {
    deposit_all->setEnabled(false);
    deposit_all->setVisible(false);
    build_bunker->setEnabled(false);
    build_bunker->setVisible(false);
  }
}

void InfoDisplay::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType ::MOVE_CHARACTER:
    {
      local_character->requestMove(displayed_tile);
      break;
    }
    case GameLib::EventType ::SCOUT_TILE:
    {
      local_character->requestScout();
      break;
    }
    case GameLib::EventType ::SCAVENGE_TILE:
    {
      local_character->requestScavenge();
      break;
    }
    case GameLib::EventType ::CLOSE_TILEINFO:
    {
      setVisible(false);
      break;
    }
    case GameLib::EventType ::BUILD_BUNKER:
    {
      data << GameLib::CharacterType ::NONE;
      Locator::instance->getNetworkManager()->sendDataToServer(data);
      break;
    }
    case GameLib::EventType ::DEPOSIT_RESOURCES:
    {
      local_character->requestDeposit(local_character->getResources());
      break;
    }
    default:
    {
      break;
    }
  }
}
