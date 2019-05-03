#include "client/Headers/UI/CharInfoDisplay.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"

CharInfoDisplay::CharInfoDisplay() :
  click_area(ClickArea(Point(1070, 140), 200, 700)),
  background_box(
    std::make_unique<AnimatedSprite>("data/PlayerInfo.png", false, false))
{
  background_box->setWidth(200);
  background_box->setHeight(500);
  background_box->xPos(1070);
  background_box->yPos(140);

  close = std::make_unique<Button>(Point(1230, 311),
                                   "data/Buttons/close.png",
                                   "data/Buttons/close2.png",
                                   GameLib::EventType::CLOSE_PLAYERINFO,
                                   25,
                                   25);
  close->addObserver(this, GameLib::EventType::CLOSE_PLAYERINFO);

  char_name = std::make_unique<TextBox>(
    Point(1110, 297), "Name", 100, 30, 1.2, ASGE::COLOURS::WHITE);

  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 384), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 425), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 466), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 507), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 549), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));
  resources_num.emplace_back(std::make_unique<TextBox>(
    Point(1120, 591), "= ", 100, 30, 1, ASGE::COLOURS::WHITE));

  char_healthbar = std::make_unique<ProgressBar>(
    Point(1241, 348), 20, 255, GameLib::Rotation::UP_TO_DOWN);
  char_healthbar->addBackgroundSprite("data/red.png");
  char_healthbar->addFillSprite("data/green.png");
  char_healthbar->addProgress(1);

  heal_character = std::make_unique<Button>(Point(1125, 645),
                                            "data/Buttons/Button_Heal.png",
                                            "data/Buttons/Button_Heal2.png",
                                            GameLib::EventType::HEAL_PLAYER,
                                            100,
                                            25);
  heal_character->addObserver(this, GameLib::EventType::HEAL_PLAYER);
  loaded = true;
}

void CharInfoDisplay::render(double delta_time)
{
  if (visible && loaded)
  {
    changeInfo();
    background_box->render(delta_time);
    char_healthbar->render(delta_time);
    char_name->render(delta_time);
    char_portrait->render(delta_time);
    close->render(delta_time);
    heal_character->render(delta_time);

    for (auto& res : resources_num)
    {
      res->render(delta_time);
    }
  }
}

void CharInfoDisplay::changeInfo()
{
  if (!character)
  {
    return;
  }

  if (character->getTile() == local_character->getTile())
  {
    heal_character->setEnabled(true);
    heal_character->setVisible(true);
  }
  else
  {
    heal_character->setEnabled(false);
    heal_character->setVisible(false);
  }

  char_healthbar->setProgress(character->getHealthPercent());

  resources_num[0]->setText("= " +
                            std::to_string(character->getResources().water));
  resources_num[1]->setText("= " +
                            std::to_string(character->getResources().food));
  resources_num[2]->setText("= " +
                            std::to_string(character->getResources().med));
  resources_num[3]->setText("= " +
                            std::to_string(character->getResources().wood));
  resources_num[4]->setText("= " +
                            std::to_string(character->getResources().metal));
  resources_num[5]->setText("= " +
                            std::to_string(character->getResources().fuel));

  if (character->getCharacterType() == GameLib::CharacterType::BUILDER &&
      current_char != GameLib::CharacterType::BUILDER)
  {
    char_name->setText("Builder");
    char_portrait = std::make_unique<AnimatedSprite>(
      "data/Portraits/Portrait_Builder.png", false, false);
  }
  else if (character->getCharacterType() == GameLib::CharacterType::MEDIC &&
           current_char != GameLib::CharacterType::MEDIC)
  {
    char_name->setText("Medic");
    char_portrait = std::make_unique<AnimatedSprite>(
      "data/Portraits/Portrait_Medic.png", false, false);
  }
  else if (character->getCharacterType() == GameLib::CharacterType::SCOUT &&
           current_char != GameLib::CharacterType::SCOUT)
  {
    char_name->setText("Scout");
    char_portrait = std::make_unique<AnimatedSprite>(
      "data/Portraits/Portrait_Scout.png", false, false);
  }
  else if (character->getCharacterType() == GameLib::CharacterType::SOLDIER &&
           current_char != GameLib::CharacterType::SOLDIER)
  {
    char_name->setText("Soldier");
    char_portrait = std::make_unique<AnimatedSprite>(
      "data/Portraits/Portrait_Soldier.png", false, false);
  }

  current_char = character->getCharacterType();
  char_portrait->xPos(1084);
  char_portrait->yPos(182);
}

void CharInfoDisplay::setVisible(bool show)
{
  visible = show;
  close->setEnabled(show);
  if (character)
  {
    character->setHighlightLock(false);
    character->setHighlighted(false);
  }
}

void CharInfoDisplay::setClickedCharacter(Character* char_clicked)
{
  if (character)
  {
    character->setHighlightLock(false);
    character->setHighlighted(false);
  }
  character = char_clicked;
  character->setHighlighted(true);
  character->setHighlightLock(true);
}

void CharInfoDisplay::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::CLOSE_PLAYERINFO)
  {
    setVisible(false);
  }
  else if (data.getType() == GameLib::EventType::HEAL_PLAYER &&
           character->getTile() == local_character->getTile())
  {
    GameLib::NetworkPacket packet(GameLib::EventType::HEAL_PLAYER);
    packet << local_character->getCharacterType()
           << character->getCharacterType();
    Locator::instance->getNetworkManager()->sendDataToServer(packet);
  }
}
