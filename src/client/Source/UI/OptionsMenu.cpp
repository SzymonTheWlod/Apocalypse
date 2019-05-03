#include "client/Headers/UI/OptionsMenu.h"
#include "client/Headers/AudioManager.h"
#include "client/Headers/FileReader.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"

using json = nlohmann::json;
OptionsMenu::OptionsMenu()
{
  json texture_paths;
  FileReader().readJsonFile("data/config/UIData.json", texture_paths);
  texture_paths = texture_paths["options"];

  background =
    std::make_unique<AnimatedSprite>(texture_paths["background"], false, false);
  background->xPos(pos.x);
  background->yPos(pos.y);
  background->setWidth(width);
  background->setHeight(height);
  music_volume =
    std::make_unique<ProgressBar>(pos + Point(90, 172.5f), 240, 35);
  music_volume->addBackgroundSprite(texture_paths["progress_bar"]["backgroun"
                                                                  "d"]);
  music_volume->addFillSprite(texture_paths["progress_bar"]["fill"]);
  music_volume->setProgress(
    Locator::instance->getAudioManager()->getMusicVolume());
  music_volume_up =
    std::make_unique<Button>(pos + Point(340, 175),
                             texture_paths["button"]["plus_default"],
                             texture_paths["button"]["plus_pressed"],
                             GameLib::EventType ::MUSIC_VOLUME_UP,
                             30,
                             30);
  music_volume_up->addObserver(this, GameLib::EventType::MUSIC_VOLUME_UP);
  music_volume_down =
    std::make_unique<Button>(pos + Point(50, 175),
                             texture_paths["button"]["minus_default"],
                             texture_paths["button"]["minus_pressed"],
                             GameLib::EventType ::MUSIC_VOLUME_DOWN,
                             30,
                             30);
  music_volume_down->addObserver(this, GameLib::EventType::MUSIC_VOLUME_DOWN);

  fx_volume = std::make_unique<ProgressBar>(pos + Point(90, 292.5f), 240, 35);
  fx_volume->addBackgroundSprite(texture_paths["progress_bar"]["background"]);
  fx_volume->addFillSprite(texture_paths["progress_bar"]["fill"]);
  fx_volume->setProgress(Locator::instance->getAudioManager()->getFXVolume());
  fx_volume_up = std::make_unique<Button>(pos + Point(340, 295),
                                          texture_paths["button"]["plus_"
                                                                  "default"],
                                          texture_paths["button"]["plus_"
                                                                  "pressed"],
                                          GameLib::EventType ::FX_VOLUME_UP,
                                          30,
                                          30);
  fx_volume_up->addObserver(this, GameLib::EventType::FX_VOLUME_UP);
  fx_volume_down = std::make_unique<Button>(pos + Point(50, 295),
                                            texture_paths["button"]["minus_"
                                                                    "default"],
                                            texture_paths["button"]["minus_"
                                                                    "pressed"],
                                            GameLib::EventType ::FX_VOLUME_DOWN,
                                            30,
                                            30);

  fx_volume_down->addObserver(this, GameLib::EventType::FX_VOLUME_DOWN);

  window_mode = std::make_unique<Button>(pos + Point(135, 360),
                                         texture_paths["button"]["default"],
                                         texture_paths["button"]["pressed"],
                                         GameLib::EventType ::WINDOW_MODE,
                                         150,
                                         50);
  window_mode->addText("");
  window_mode->changeTextColour(ASGE::COLOURS::WHITE);
  window_mode->addObserver(this, GameLib::EventType::WINDOW_MODE);
  updateModeText();

  close = std::make_unique<Button>(pos + Point(380, 9),
                                   texture_paths["button"]["close_default"],
                                   texture_paths["button"]["close_pressed"],
                                   GameLib::EventType ::CLOSE_WINDOW,
                                   30,
                                   30);

  close->addObserver(this, GameLib::EventType::CLOSE_WINDOW);

  exit_menu = std::make_unique<Button>(pos + Point(7, 425),
                                       texture_paths["button"]["menu_default"],
                                       texture_paths["button"]["menu_pressed"],
                                       GameLib::EventType ::BACK_TO_MENU,
                                       200,
                                       40);
  exit_game = std::make_unique<Button>(pos + Point(211, 425),
                                       texture_paths["button"]["exit_default"],
                                       texture_paths["button"]["exit_pressed"],
                                       GameLib::EventType ::EXIT_GAME,
                                       200,
                                       40);
  hide();
}

void OptionsMenu::render(double delta_time)
{
  if (active)
  {
    background->render(delta_time);
    music_volume->render(delta_time);
    music_volume_down->render(delta_time);
    music_volume_up->render(delta_time);
    fx_volume->render(delta_time);
    fx_volume_down->render(delta_time);
    fx_volume_up->render(delta_time);
    window_mode->render(delta_time);
    close->render(delta_time);

    if (use_exit_buttons)
    {
      exit_game->render(delta_time);
      exit_menu->render(delta_time);
    }
  }
}

void OptionsMenu::onNotify(GameLib::NetworkPacket& data)
{
  AudioManager* audio = Locator::instance->getAudioManager();
  switch (data.getType())
  {
    case GameLib::EventType ::MUSIC_VOLUME_UP:
    {
      audio->setMusicVolume(audio->getMusicVolume() + 0.1f);
      break;
    }
    case GameLib::EventType ::MUSIC_VOLUME_DOWN:
    {
      audio->setMusicVolume(audio->getMusicVolume() - 0.1f);
      break;
    }
    case GameLib::EventType ::FX_VOLUME_UP:
    {
      audio->setFXVolume(audio->getFXVolume() + 0.1f);
      break;
    }
    case GameLib::EventType ::FX_VOLUME_DOWN:
    {
      audio->setFXVolume(audio->getFXVolume() - 0.1f);
      break;
    }
    case GameLib::EventType ::CLOSE_WINDOW:
    {
      hide();
      break;
    }
    case GameLib::EventType ::WINDOW_MODE:
    {
      Locator::instance->getRenderer()->cycleWindowMode();
      updateModeText();
      break;
    }
    default:
    {
      break;
    }
  }
  music_volume->setProgress(audio->getMusicVolume());
  fx_volume->setProgress(audio->getFXVolume());
}

void OptionsMenu::show()
{
  fx_volume_up->setEnabled(true);
  fx_volume_down->setEnabled(true);
  music_volume_up->setEnabled(true);
  music_volume_down->setEnabled(true);
  close->setEnabled(true);
  active = true;
  window_mode->setEnabled(true);
  if (use_exit_buttons)
  {
    exit_game->setEnabled(true);
    exit_menu->setEnabled(true);
  }
}

void OptionsMenu::hide()
{
  fx_volume_up->setEnabled(false);
  fx_volume_down->setEnabled(false);
  music_volume_up->setEnabled(false);
  music_volume_down->setEnabled(false);
  close->setEnabled(false);
  exit_game->setEnabled(false);
  exit_menu->setEnabled(false);
  window_mode->setEnabled(false);
  active = false;
}

void OptionsMenu::updateModeText()
{
  if (Locator::instance->getRenderer()->getWindowMode() ==
      ASGE::Renderer::WindowMode::WINDOWED)
  {
    window_mode->editText("Fullscreen");
  }
  else if (Locator::instance->getRenderer()->getWindowMode() ==
           ASGE::Renderer::WindowMode::BORDERLESS)
  {
    window_mode->editText("Windowed");
  }
}

void OptionsMenu::addSceneChangeObserver(GameLib::Observer* observer)
{
  exit_menu->addObserver(observer, GameLib::EventType::BACK_TO_MENU);
  exit_game->addObserver(observer, GameLib::EventType::EXIT_GAME);
}