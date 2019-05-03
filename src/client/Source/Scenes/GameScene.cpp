#include "client/Headers/AudioManager.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
#include <client/Headers/Scenes/GameScene.h>

GameScene::~GameScene()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_CHARACTER);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::SET_WIN_CONDITION);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType ::LOSE_GAME);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType ::WIN_GAME);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType ::CHAT_MESSAGE);

  Locator::instance->getInputManager()->removeObserver(
    this, GameLib::EventType ::MOUSE_SCROLLED);
}

bool GameScene::load()
{
  loadAllSounds();
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_RECONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_CHARACTER);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::SET_WIN_CONDITION);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType ::LOSE_GAME);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType ::WIN_GAME);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType ::CHAT_MESSAGE);

  Locator::instance->getInputManager()->addObserver(
    this, GameLib::EventType ::MOUSE_SCROLLED);

  lose_screen =
    std::make_unique<AnimatedSprite>("data/FailScreen.png", false, false);
  win_screen =
    std::make_unique<AnimatedSprite>("data/WinScreen.png", false, false);
  disconnected_overlay =
    std::make_unique<AnimatedSprite>("data/Reconnect.jpg", false, false);

  map = std::make_unique<Map>();
  character_manager.setLocalCharacter(scene_data.selected_char);
  character_manager.setMap(map.get());
  ui_manager.setCharManager(&character_manager);
  ui_manager.setMap(map.get());
  options_menu.useExitButtons(true);
  options_menu.addSceneChangeObserver(this);

  chat_window = std::make_unique<ChatWindow>(
    Point(GameLib::GAME_WIDTH - 650, GameLib::GAME_HEIGHT - 300),
    400,
    250,
    1,
    ASGE::COLOURS::WHITE);
  chat_window->setBackgroundSprite("data/ChatOutput.png");
  chat_window->setUsername(scene_data.username);

  chat_input = std::make_unique<TextInput>(
    Point(GameLib::GAME_WIDTH - 650, GameLib::GAME_HEIGHT - 50),
    "Click to chat...",
    GameLib::EventType ::INPUT_SUBMITTED,
    400,
    50,
    1,
    ASGE::COLOURS::WHITE);
  chat_input->addObserver(chat_window.get(),
                          GameLib::EventType::INPUT_SUBMITTED);
  chat_input->setClearOnSubmit(true);
  chat_input->setBackgroundSprite("data/ChatInput.png");
  chat_title = std::make_unique<Image>(
    "data/ChatText.png",
    Point(GameLib::GAME_WIDTH - 630, GameLib::GAME_HEIGHT - 340));

  hide_chat = std::make_unique<Button>(
    Point(GameLib::GAME_WIDTH - 690, GameLib::GAME_HEIGHT - 35),
    "data/Buttons/Minus.png",
    "data/Buttons/Minus2.png",
    GameLib::EventType::HIDE_CHAT,
    25,
    25);
  hide_chat->addObserver(this, GameLib::EventType::HIDE_CHAT);
  show_chat = std::make_unique<Button>(
    Point(GameLib::GAME_WIDTH - 690, GameLib::GAME_HEIGHT - 35),
    "data/Buttons/Plus.png",
    "data/Buttons/Plus2.png",
    GameLib::EventType::SHOW_CHAT,
    25,
    25);
  show_chat->addObserver(this, GameLib::EventType::SHOW_CHAT);
  show_chat->setVisible(false);
  show_chat->setEnabled(false);
  return true;
}

void GameScene::loadAllSounds()
{
  Locator::instance->getAudioManager()->unloadAllSoundFiles();
  Locator::instance->getAudioManager()->loadMusic(
    GameLib::MusicTrack::BACKGROUND_TRACK);
  Locator::instance->getAudioManager()->playMusic(
    GameLib::MusicTrack::BACKGROUND_TRACK);

  Locator::instance->getAudioManager()->loadSoundFX(
    GameLib::SoundFX::AVALANCHE);
  Locator::instance->getAudioManager()->loadSoundFX(
    GameLib::SoundFX::EXPLOSION);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::HEAL);

  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::MOVE);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::SCAVANGE);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::SCOUT);

  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::THUNDER);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::TORNADO);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::TREMER);

  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::TSUNAMI);
  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::WILDFIRE);
  Locator::instance->getAudioManager()->loadSoundFX(
    GameLib::SoundFX::YOUR_TURN);

  Locator::instance->getAudioManager()->loadSoundFX(GameLib::SoundFX::METEOR);

  // Load all FX
}

void GameScene::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE &&
      key->action == ASGE::KEYS::KEY_RELEASED && !ui_manager.closeWindows())
  {
    if (options_menu.visible())
    {
      options_menu.hide();
    }
    else
    {
      options_menu.show();
    }
  }
  else
  {
    cameraControls(key);
  }
}

void GameScene::cameraControls(const ASGE::KeyEvent* key)
{
  bool released = key->action == ASGE::KEYS::KEY_RELEASED;
  switch (key->key)
  {
    case ASGE::KEYS::KEY_S:
    {
      camera_move.y = released ? 0 : 1;
      break;
    }
    case ASGE::KEYS::KEY_W:
    {
      camera_move.y = released ? 0 : -1;
      break;
    }
    case ASGE::KEYS::KEY_D:
    {
      camera_move.x = released ? 0 : 1;
      break;
    }
    case ASGE::KEYS::KEY_A:
    {
      camera_move.x = released ? 0 : -1;
      break;
    }
    default:
    {
      break;
    }
  }
  // Stop camera movement while typing in chat
  if (chat_input->hasFocus())
  {
    camera_move.x = 0;
    camera_move.y = 0;
  }
}

void GameScene::mouseHandler(ASGE::ClickEvent& click)
{
  if (click.action != ASGE::MOUSE::BUTTON_RELEASED)
  {
    return;
  }
  Point click_pos =
    Point(static_cast<float>(click.xpos), static_cast<float>(click.ypos));
  // If the click is not on the UI
  if (!ui_manager.doesClickHitUI(
        Locator::instance->getInputManager()->getScreenMousePosition()))
  {
    Tile* tile_clicked = map->checkIfClicked(click_pos);
    Character* char_clicked = character_manager.checkPlayerClicked(click_pos);

    if (char_clicked)
    {
      ui_manager.showCharInfo(true);
      ui_manager.displayCharInformation(char_clicked);
      ui_manager.showTileInfo(true);
      ui_manager.displayTileInformation(tile_clicked);
    }
    else if (tile_clicked)
    {
      ui_manager.showTileInfo(true);
      ui_manager.displayTileInformation(tile_clicked);
    }
  }
}

GameLib::SceneType GameScene::update(double delta_time)
{
  Locator::instance->getRenderer()->moveCamera(
    camera_move * (static_cast<float>(delta_time) / 2));
  clientEvent.updateEvents();
  ui_manager.displayStockpileInfo();
  map->update();
  character_manager.update(delta_time);
  return next_scene;
}

void GameScene::render(double delta_time)
{
  map->render(delta_time);
  character_manager.render(delta_time);
  clientEvent.render(delta_time);
  ui_manager.render(delta_time);

  chat_window->render(delta_time);
  chat_input->render(delta_time);
  chat_title->render(delta_time);
  show_chat->render(delta_time);
  hide_chat->render(delta_time);

  if (waiting_for_reconnect)
  {
    disconnected_overlay->render(delta_time);
  }

  if (won_game)
  {
    win_screen->render(delta_time);
  }
  else if (lost_game)
  {
    lose_screen->render(delta_time);
  }

  options_menu.render(delta_time);
}

void GameScene::onNotify(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType ::MOUSE_SCROLLED:
    {
      Point pos = Locator::instance->getInputManager()->getWorldMousePosition();
      double scroll_amount;
      data >> scroll_amount;
      Locator::instance->getRenderer()->zoomCamera(
        static_cast<float>(scroll_amount / 10), pos);
      break;
    }
    case GameLib::EventType ::CLIENT_DISCONNECTED:
    {
      waiting_for_reconnect = true;
      break;
    }
    case GameLib::EventType ::CLIENT_RECONNECTED:
    {
      waiting_for_reconnect = false;
      break;
    }
    case GameLib::EventType ::SET_CHARACTER:
    {
      data >> scene_data.selected_char;
      character_manager.setLocalCharacter(scene_data.selected_char);
      break;
    }
    case GameLib::EventType::BACK_TO_MENU:
    {
      next_scene = GameLib::SceneType ::MENU_SCENE;
      break;
    }
    case GameLib::EventType ::EXIT_GAME:
    {
      Locator::instance->getNetworkManager()->disconnect();
      next_scene = GameLib::SceneType ::SIGNAL_EXIT;
      break;
    }
    case GameLib::EventType ::LOSE_GAME:
    {
      lost_game = true;
      break;
    }
    case GameLib::EventType ::WIN_GAME:
    {
      won_game = true;
      break;
    }
    case GameLib::EventType ::SET_WIN_CONDITION:
    {
      Resources win_amount;
      data >> win_amount;
      ui_manager.setWinAmount(win_amount);
      break;
    }
    default:
    {
      chatEvents(data);
      break;
    }
  }
}

void GameScene::chatEvents(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType ::CHAT_MESSAGE:
    {
      std::string msg;
      data >> msg;
      chat_window->pushMessage(msg);
      break;
    }
    case GameLib::EventType ::SHOW_CHAT:
    {
      show_chat->setEnabled(false);
      show_chat->setVisible(false);
      chat_title->setVisible(true);
      chat_window->setVisible(true);
      hide_chat->setEnabled(true);
      hide_chat->setVisible(true);
      break;
    }
    case GameLib::EventType ::HIDE_CHAT:
    {
      show_chat->setEnabled(true);
      show_chat->setVisible(true);
      chat_title->setVisible(false);
      chat_window->setVisible(false);
      hide_chat->setEnabled(false);
      hide_chat->setVisible(false);
      break;
    }
    default:
    {
      break;
    }
  }
}