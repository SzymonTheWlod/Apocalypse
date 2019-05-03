#include "client/Headers/ClientEvent.h"
#include "client/Headers/AudioManager.h"
#include "client/Headers/FileReader.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
#include <json.hpp>

using json = nlohmann::json;
ClientEvent::ClientEvent()
{
  // Instantiate the Danger Bar
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::DISASTER_BAR);
  sprite = std::make_unique<ProgressBar>(
    Point(35, 92), 30, 440, GameLib::Rotation::UP_TO_DOWN);

  sprite->addBackgroundSprite("data/default_white.png");
  sprite->addFillSprite("data/red.png");
  danger_meter =
    std::make_unique<AnimatedSprite>("data/Thermometer.png", false, false);
  danger_meter->yPos(20);
  danger_meter->xPos(10);

  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::EVENTS);
}

ClientEvent::~ClientEvent()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::DISASTER_BAR);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::EVENTS);
}

void ClientEvent::onNotify(GameLib::NetworkPacket& data)
{
  Point start_pos;
  Point end_pos;

  if (data.getType() == GameLib::EventType::DISASTER_BAR)
  {
    data >> barIncrement;
    sprite->addProgress(static_cast<float>(barIncrement));
  }

  if (data.getType() == GameLib::EventType::EVENTS)
  {
    int event_id = 0;
    data >> event_id >> start_pos >> end_pos;
    std::string str_id = std::to_string(event_id);
    json config;
    FileReader().readJsonFile("data/config/EventData.json", config);
    config = config["event_id"][str_id];

    auto sound = static_cast<GameLib::SoundFX>(config["sound_effect"]);
    Locator::instance->getAudioManager()->playSoundFX(sound);
    event_sprite = std::make_unique<AnimatedSprite>(true, true);
    for (json& file_path : config["file_paths"])
    {
      event_sprite->addSprite(file_path);
    }
    event_sprite->timeBetweenFrames(config["anim_speed"]);
    event_sprite->setHeight(config["height"]);
    event_sprite->setWidth(config["width"]);

    event_sprite->xPos(start_pos.x);
    event_sprite->yPos(start_pos.y);

    event_sprite->lerpTo(end_pos.x, end_pos.y, config["duration"]);
  }
}

void ClientEvent::updateEvents()
{
  if (event_sprite && !event_sprite->isLerping())
  {
    event_sprite.reset();
  }
}
void ClientEvent::render(double delta_time)
{
  if (event_sprite)
  {
    event_sprite->render(delta_time);
  }
  danger_meter->render(delta_time);
  sprite->render(delta_time);
}
