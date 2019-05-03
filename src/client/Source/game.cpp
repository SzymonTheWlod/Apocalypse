#include "client/Headers/game.h"
#include "client/Headers/FileReader.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Scenes/TitleScene.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/NetworkPacket.h"

#include <fstream>
#include <json.hpp>
#include <sstream>

#include <Engine/DebugPrinter.h>
#include <Engine/FileIO.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

using json = nlohmann::json;

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
MyASGEGame::MyASGEGame()
{
  game_name = "ASGE Game";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
MyASGEGame::~MyASGEGame()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));

  // Reset the scene manager first so it doesn't seg fault on removing observers
  scene_manager.reset();
  network_manager.reset();
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool MyASGEGame::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }
  initAPI();

  // toggleFPS();

  Locator::instance = std::make_unique<Locator>();

  game_renderer = std::make_unique<GameRenderer>(renderer.get());
  Locator::instance->setRenderer(game_renderer.get());

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &MyASGEGame::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &MyASGEGame::clickHandler, this);

  scroll_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_SCROLL, &MyASGEGame::scrollHandler, this);

  network_manager = std::make_unique<NetworkManager>();
  Locator::instance->setNetworkManager(network_manager.get());

  scene_manager = std::make_unique<SceneManager>();
  network_manager->addObserver(scene_manager.get(),
                               GameLib::EventType::SERVER_CHANGE_SCENE);

  scene_manager->changeScene(GameLib::SceneType::TITLE_SCENE);

  input_manager = std::make_unique<InputManager>();
  input_manager->setInput(inputs.get());
  Locator::instance->setInputManager(input_manager.get());

  audio_manager = std::make_unique<AudioManager>();
  Locator::instance->setAudioManager(audio_manager.get());

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void MyASGEGame::setupResolution()
{
  json json_file;
  FileReader().readJsonFile("data/config/GameSetup.json", json_file);
  game_width = json_file["game_width"];
  game_height =
    static_cast<int>(static_cast<float>(game_width) / GameLib::ASPECT_RATIO);

  AnimatedSprite::width_scale =
    static_cast<float>(game_width) / static_cast<float>(GameLib::GAME_WIDTH);
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void MyASGEGame::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());
  Locator::instance->getInputManager()->processKeyData(key);
  scene_manager->sceneKeyHandler(data);
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void MyASGEGame::clickHandler(const ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());
  // Recreate this click event as non const so we can apply the scale offset
  ASGE::ClickEvent offset_click;
  Point offset =
    Point(static_cast<float>(click->xpos), static_cast<float>(click->ypos)) /
    AnimatedSprite::width_scale;
  offset_click.xpos = offset.x;
  offset_click.ypos = offset.y;
  offset_click.button = click->button;
  offset_click.action = click->action;
  offset_click.mods = click->mods;

  ASGE::ClickEvent world_click = offset_click;
  offset = game_renderer->displayedToSimulatedWorld(
    Point(static_cast<float>(click->xpos), static_cast<float>(click->ypos)));
  world_click.xpos = offset.x;
  world_click.ypos = offset.y;
  // Send world space click to the scene which it uses
  scene_manager->sceneMouseHandler(world_click);

  // Send screen space click to the event system which the UI uses
  Locator::instance->getInputManager()->processClickData(&offset_click);
}

void MyASGEGame::scrollHandler(const ASGE::SharedEventData data)
{
  auto scroll = static_cast<const ASGE::ScrollEvent*>(data.get());
  Locator::instance->getInputManager()->processScrollData(scroll);
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void MyASGEGame::update(const ASGE::GameTime& game_time)
{
  GameLib::SceneType type =
    scene_manager->updateCurrentScene(game_time.delta.count());

  if (type == GameLib::SceneType::SIGNAL_EXIT)
  {
    signalExit();
  }

  network_manager->update();
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void MyASGEGame::render(const ASGE::GameTime& game_time)
{
  scene_manager->renderCurrentScene(game_time.delta.count());
}
