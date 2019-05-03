#include "client/Headers/Scenes/CharacterSelect.h"
#include "client/Headers/FileReader.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Network/NetworkManager.h"
#include "client/Headers/UI/Image.h"
#include <client/Headers/Locator.h>

using json = nlohmann::json;

CharacterSelect::~CharacterSelect()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLAIM_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
}

bool CharacterSelect::load()
{
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLAIM_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);

  background = std::make_unique<AnimatedSprite>("data/CharacterScreen.png");

  disconnected_overlay = std::make_unique<AnimatedSprite>("data/Reconnect.jpg");

  json json_file;
  FileReader().readJsonFile("data/config/CharacterData.json", json_file);

  std::string description;
  description = json_file["scout"]["description"];
  scout_info = std::make_unique<TextBox>(
    Point(25, 550), description, 250, 400, 1, ASGE::COLOURS::WHITE);
  description = json_file["medic"]["description"];
  medic_info = std::make_unique<TextBox>(
    Point(325, 550), description, 250, 400, 1, ASGE::COLOURS::WHITE);
  description = json_file["builder"]["description"];
  builder_info = std::make_unique<TextBox>(
    Point(625, 550), description, 250, 400, 1, ASGE::COLOURS::WHITE);
  description = json_file["soldier"]["description"];
  soldier_info = std::make_unique<TextBox>(
    Point(925, 550), description, 250, 400, 1, ASGE::COLOURS::WHITE);

  loadButtons();
  return true;
}

void CharacterSelect::loadButtons()
{
  scout = std::make_unique<Button>(Point(20, 20),
                                   "data/Character_Select/Scouter1.png",
                                   "data/Character_Select/Scouter2.png",
                                   GameLib::EventType::SCOUT_CHOSEN,
                                   220,
                                   540);
  dynamic_cast<Button*>(scout.get())
    ->addObserver(this, GameLib::EventType::SCOUT_CHOSEN);

  medic = std::make_unique<Button>(Point(259, 20),
                                   "data/Character_Select/Medic1.png",
                                   "data/Character_Select/Medic2.png",
                                   GameLib::EventType::MEDIC_CHOSEN,
                                   300,
                                   540);
  dynamic_cast<Button*>(medic.get())
    ->addObserver(this, GameLib::EventType::MEDIC_CHOSEN);

  builder = std::make_unique<Button>(Point(578, 20),
                                     "data/Character_Select/Builder1.png",
                                     "data/Character_Select/Builder2.png",
                                     GameLib::EventType::BUILDER_CHOSEN,
                                     352,
                                     540);
  dynamic_cast<Button*>(builder.get())
    ->addObserver(this, GameLib::EventType::BUILDER_CHOSEN);

  soldier = std::make_unique<Button>(Point(949, 20),
                                     "data/Character_Select/Soldier1.png",
                                     "data/Character_Select/Soldier2.png",
                                     GameLib::EventType::SOLDIER_CHOSEN,
                                     309,
                                     540);
  dynamic_cast<Button*>(soldier.get())
    ->addObserver(this, GameLib::EventType::SOLDIER_CHOSEN);
}

void CharacterSelect::keyHandler(ASGE::SharedEventData data) {}

void CharacterSelect::mouseHandler(ASGE::ClickEvent& click) {}

GameLib::SceneType CharacterSelect::update(double delta_time)
{
  return next_scene;
}

void CharacterSelect::render(double delta_time)
{
  background->render(delta_time);
  scout->render(delta_time);
  medic->render(delta_time);
  builder->render(delta_time);
  soldier->render(delta_time);

  scout_info->render(delta_time);
  medic_info->render(delta_time);
  builder_info->render(delta_time);
  soldier_info->render(delta_time);

  if (waiting_for_reconnect)
  {
    disconnected_overlay->render(delta_time);
  }
}

void CharacterSelect::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType::SET_CHARACTER:
    {
      GameLib::CharacterType type;
      data >> type;
      removeCharacter(type, "data/Character_Picked");
      scene_data.selected_char = type;
      break;
    }
    case GameLib::EventType::CLAIM_CHARACTER:
    {
      GameLib::CharacterType type;
      data >> type;
      removeCharacter(type, "data/Character_Taken");
      break;
    }
    case GameLib::EventType::CLIENT_DISCONNECTED:
    {
      waiting_for_reconnect = true;
      break;
    }
    case GameLib::EventType::CLIENT_RECONNECTED:
    {
      waiting_for_reconnect = false;
      break;
    }
    default:
    {
      Locator::instance->getNetworkManager()->sendDataToServer(data);
      break;
    }
  }
}

void CharacterSelect::removeCharacter(GameLib::CharacterType& type,
                                      const std::string& replacement_texture)
{
  switch (type)
  {
    case GameLib::CharacterType::MEDIC:
    {
      medic = std::make_unique<Image>(replacement_texture + "/Medic.png",
                                      medic->getPosition());
      break;
    }
    case GameLib::CharacterType::SCOUT:
    {
      scout = std::make_unique<Image>(replacement_texture + "/Scout.png",
                                      scout->getPosition());
      break;
    }
    case GameLib::CharacterType::SOLDIER:
    {
      soldier = std::make_unique<Image>(replacement_texture + +"/Soldier.png",
                                        soldier->getPosition());
      break;
    }
    case GameLib::CharacterType::BUILDER:
    {
      builder = std::make_unique<Image>(replacement_texture + +"/Builder.png",
                                        builder->getPosition());
      break;
    }
    default:
    {
      break;
    }
  }
}
