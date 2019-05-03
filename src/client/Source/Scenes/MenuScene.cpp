#include <Engine/Renderer.h>
#include <string>

#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>

#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/AudioManager.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Network/NetworkManager.h"
#include "client/Headers/Scenes/MenuScene.h"
#include "gamelib/Headers/Constants.h"

MenuScene::~MenuScene()
{
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_CONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::NUMBER_OF_CLIENTS_READY);
  Locator::instance->getNetworkManager()->removeObserver(
    this, GameLib::EventType::CHAT_MESSAGE);
}

/**
*   @brief   Loads all variables and sprites for this scene
*   @details Initialises all variables and creates all the new
sprites for the scene
*/
bool MenuScene::load()
{
  Locator::instance->getRenderer()->resetCamera();
  Locator::instance->getNetworkManager()->disconnect();

  Locator::instance->getRenderer()->setClearColour(ASGE::COLOURS::BLACK);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_CONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CLIENT_DISCONNECTED);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::NUMBER_OF_CLIENTS_READY);
  Locator::instance->getNetworkManager()->addObserver(
    this, GameLib::EventType::CHAT_MESSAGE);

  Locator::instance->getAudioManager()->loadMusic(
    GameLib::MusicTrack ::MENU_TRACK);
  Locator::instance->getAudioManager()->playMusic(
    GameLib::MusicTrack::MENU_TRACK, 1, true);

  background = std::make_unique<AnimatedSprite>("data/menu.jpg");

  instructions_panel = std::make_unique<AnimatedSprite>("data/"
                                                        "instructions_window."
                                                        "png");
  instructions_panel->setWidth(459);
  instructions_panel->setHeight(700);
  instructions_panel->xPos(410);
  instructions_panel->yPos(10);

  username = std::make_unique<TextInput>(Point(1000, 100),
                                         "Enter username...",
                                         GameLib::EventType ::SET_USERNAME,
                                         400,
                                         40,
                                         1,
                                         ASGE::COLOURS::WHITE);
  username->addObserver(this, GameLib::EventType::SET_USERNAME);

  ip_input = std::make_unique<TextInput>(Point(1000, 150),
                                         "Enter ip...",
                                         GameLib::EventType ::INPUT_SUBMITTED,
                                         200,
                                         40,
                                         1,
                                         ASGE::COLOURS::WHITE);
  ip_input->addObserver(this, GameLib::EventType::INPUT_SUBMITTED);
  ip_input->setClearOnSubmit(true);

  is_connected = std::make_unique<TextBox>(
    Point(1000, 180), "NOT CONNECTED", 200, 40, 1, ASGE::COLOURS::WHITE);

  ready_up = std::make_unique<Button>(Point(1007, 250),
                                      "data/Buttons/Button.png",
                                      "data/Buttons/Button2.png",
                                      GameLib::EventType::CLIENT_READY_TO_START,
                                      150,
                                      40);
  ready_up->addObserver(this, GameLib::EventType::CLIENT_READY_TO_START);

  is_ready = std::make_unique<TextBox>(
    Point(1018, 247), "NOT READY", 200, 100, 1, ASGE::COLOURS::WHITE);
  players_ready = std::make_unique<TextBox>(
    Point(1018, 282), "0/1 READY", 200, 100, 1, ASGE::COLOURS::WHITE);

  chat_window = std::make_unique<ChatWindow>(
    Point(GameLib::GAME_WIDTH - 400, GameLib::GAME_HEIGHT - 300),
    400,
    250,
    1,
    ASGE::COLOURS::WHITE);

  chat_input = std::make_unique<TextInput>(
    Point(GameLib::GAME_WIDTH - 400, GameLib::GAME_HEIGHT - 50),
    "Click to chat...",
    GameLib::EventType ::INPUT_SUBMITTED,
    400,
    50,
    1,
    ASGE::COLOURS::WHITE);
  chat_input->addObserver(chat_window.get(),
                          GameLib::EventType::INPUT_SUBMITTED);
  chat_input->setClearOnSubmit(true);

  open_options = std::make_unique<Button>(Point(10, 670),
                                          "data/Buttons/Options_button.png",
                                          "data/Buttons/Options_button2.png",
                                          GameLib::EventType::OPEN_WINDOW,
                                          150,
                                          40);
  open_options->addObserver(this, GameLib::EventType::OPEN_WINDOW);

  exit_game = std::make_unique<Button>(Point(170, 670),
                                       "data/Buttons/Exit.png",
                                       "data/Buttons/Exit2.png",
                                       GameLib::EventType::EXIT_GAME,
                                       150,
                                       40);
  exit_game->addObserver(this, GameLib::EventType::EXIT_GAME);

  instructions = std::make_unique<Button>(Point(10, 620),
                                          "data/Buttons/Instructions.png",
                                          "data/Buttons/Instructions2.png",
                                          GameLib::EventType::OPEN_INSTRUCTIONS,
                                          150,
                                          40);
  instructions->addObserver(this, GameLib::EventType::OPEN_INSTRUCTIONS);
  return true;
}

/**
 *   @brief   Changes game state based on inputs
 *   @details Is called for every input event and will
 *   @details change the game state / variables etc depending
 *   @param   data is the event
 */
void MenuScene::keyHandler(const ASGE::SharedEventData data) {}

/**
 *   @brief   Changes game state based on mouse inputs
 *   @details Is called for every input event and will change
 *   @details the game state / variables etc depending
 *   @param   data is the event, mouse_position the position of the cursor
 */
void MenuScene::mouseHandler(ASGE::ClickEvent& click) {}

/**
 *   @brief   Updates all variables for this scene
 *   @details Runs every frame and handles all the logic and updates
 *			 of the active scene
 *   @param  delta_time is time since last update
 *   @return  number of the scene to switch to, -1 no change, -2 exit game
 */
GameLib::SceneType MenuScene::update(double delta_time)
{
  if (Locator::instance->getNetworkManager()->isConnected())
  {
    is_connected->setText("CONNECTED");
  }
  else
  {
    is_connected->setText("NOT CONNECTED");
  }
  return next_scene;
}

/**
*   @brief   Renders all sprites for this scene
*   @details Runs every frame and draws all the sprites in
order
*/
void MenuScene::render(double delta_time)
{
  background->render(delta_time);
  ip_input->render(delta_time);
  is_connected->render(delta_time);
  username->render(delta_time);
  ready_up->render(delta_time);
  is_ready->render(delta_time);

  chat_window->render(delta_time);
  chat_input->render(delta_time);
  players_ready->render(delta_time);

  open_options->render(delta_time);
  options_menu.render(delta_time);

  exit_game->render(delta_time);
  instructions->render(delta_time);

  if (instructions_open)
  {
    instructions_panel->render(delta_time);
  }
}

void MenuScene::onNotify(GameLib::NetworkPacket& data)
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
    case GameLib::EventType ::SET_USERNAME:
    {
      std::string new_user;
      data >> new_user;
      scene_data.username = new_user;
      chat_window->setUsername(new_user);
      break;
    }
    case GameLib::EventType ::INPUT_SUBMITTED:
    {
      std::string ip;
      data >> ip;
      Locator::instance->getNetworkManager()->tryConnection(ip.c_str());
      break;
    }
    case GameLib::EventType ::OPEN_WINDOW:
    {
      options_menu.show();
      break;
    }
    case GameLib::EventType ::OPEN_INSTRUCTIONS:
    {
      instructions_open = !instructions_open;
      break;
    }
    case GameLib::EventType ::EXIT_GAME:
    {
      next_scene = GameLib::SceneType ::SIGNAL_EXIT;
      break;
    }
    default:
    {
      connectionPackets(data);
      break;
    }
  }
}

void MenuScene::connectionPackets(GameLib::NetworkPacket& data)
{
  switch (data.getType())
  {
    case GameLib::EventType::CLIENT_CONNECTED:
    {
      ready = false;
      is_ready->setText("NOT READY");
      break;
    }
    case GameLib::EventType::CLIENT_DISCONNECTED:
    {
      ready = false;
      is_ready->setText("NOT READY");
      break;
    }
    case GameLib::EventType::CLIENT_READY_TO_START:
    {
      if (Locator::instance->getNetworkManager()->isConnected() && !ready)
      {
        ready = true;
        is_ready->setText("READY");
        Locator::instance->getNetworkManager()->sendDataToServer(data);
      }
      break;
    }
    case GameLib::EventType::NUMBER_OF_CLIENTS_READY:
    {
      int num_ready;
      int total;
      data >> num_ready;
      data >> total;
      std::string new_str =
        std::to_string(num_ready) + "/" + std::to_string(total) + " READY";
      players_ready->setText(new_str);
      break;
    }
    default:
    {
      break;
    }
  }
}