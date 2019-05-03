#include "client/Headers/Scenes/TitleScene.h"
#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/GameRenderer.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Point.h"

#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Renderer.h>
#include <client/Headers/Locator.h>
#include <soloud.h>

/**
*   @brief   Loads all variables and sprites for this scene
*   @details Initialises all variables and creates all the new
                         sprites for the scene
*/
bool TitleScene::load()
{
  Locator::instance->getRenderer()->setClearColour(ASGE::COLOURS::BLACK);
  title = std::make_unique<AnimatedSprite>("data/Title.jpg");
  title->setColour(ASGE::COLOURS::BLACK);
  title->fadeToColour(ASGE::COLOURS::WHITE, 1000);

  return true;
}

/**
 *   @brief   Changes game state based on inputs
 *   @details Is called for every input event and will change
 *   @details the game state / variables etc depending
 *   @param   data is the event
 */
void TitleScene::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());
  if (key->key)
  {
    next_scene = GameLib::SceneType::MENU_SCENE;
  }
}

/**
 *   @brief   Changes game state based on mouse inputs
 *   @details Is called for every input event and will change
 *   @details the game state / variables etc depending
 *   @param   data is the event, mouse_position the position of the cursor
 */
void TitleScene::mouseHandler(ASGE::ClickEvent& click) {}

/**
 *   @brief   Updates all variables for this scene
 *   @details Runs every frame and handles all the logic and updates
 *			 of the active scene
 *   @param  delta_time is time since last update
 *   @return  number of the scene to switch to, -1 no change, -2 exit game
 */
GameLib::SceneType TitleScene::update(double delta_time)
{
  time_waited += delta_time;
  if (time_waited > 1500)
  {
    next_scene = GameLib::SceneType::MENU_SCENE;
  }

  return next_scene;
}

/**
*   @brief   Renders all sprites for this scene
*   @details Runs every frame and draws all the sprites in
                         order
*/
void TitleScene::render(double delta_time)
{
  title->render(delta_time);
}