#include "client/Headers/Scenes/SceneManager.h"
#include "client/Headers/game.h"

#include "client/Headers/Scenes/CharacterSelect.h"
#include "client/Headers/Scenes/GameScene.h"
#include "client/Headers/Scenes/MenuScene.h"
#include "client/Headers/Scenes/TitleScene.h"

/**
 *   @brief   Loads the active scene
 *   @details Calls loadScene() on scene that is currently active
 *   @param   renderer is a pointer to the renderer
 */
bool SceneManager::loadCurrentScene()
{
  return current_scene->load();
}

/**
 *   @brief   Handles inputs of the active scene
 *   @param   data is the event
 */
void SceneManager::sceneKeyHandler(const ASGE::SharedEventData data)
{
  current_scene->keyHandler(data);
}

/**
 *   @brief   Handles mouse clicks and movement of the active scene
 *   @param   data is the eventm mouse_position is the position of the cursor on
 * screen
 */
void SceneManager::sceneMouseHandler(ASGE::ClickEvent& click)
{
  current_scene->mouseHandler(click);
}

/**
 *   @brief   Updates the active scene
 *   @details Calls updateScene() on scene that is currently active
 *   @param   delta_time is time since last update
 *   @return  1 if the scene has changed, 0 if it hasn't and -1 signals to exit
 */
GameLib::SceneType SceneManager::updateCurrentScene(double delta_time)
{
  GameLib::SceneType type = current_scene->update(delta_time);

  if (type == GameLib::SceneType::SIGNAL_EXIT)
  {
    return GameLib::SceneType::SIGNAL_EXIT;
  }
  else if (type == GameLib::SceneType::NO_CHANGE)
  {
    return GameLib::SceneType::NO_CHANGE;
  }
  changeScene(type);
  return type;
}

/**
 *   @brief   Renders the active scene
 *   @details Calls renderScene() on scene that is currently active
 *   @param   renderer is a pointer to the renderer
 *   @param  delta_time is time since last update
 */
void SceneManager::renderCurrentScene(double delta_time)
{
  current_scene->render(delta_time);
}

void SceneManager::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::SERVER_CHANGE_SCENE)
  {
    GameLib::SceneType scene_type;
    data >> scene_type;
    changeScene(scene_type);
  }
}

void SceneManager::changeScene(GameLib::SceneType type)
{
  PersistentClientData scene_data;
  if (current_scene)
  {
    scene_data = current_scene->getPersistentData();
  }
  switch (type)
  {
    case GameLib::SceneType::TITLE_SCENE:
    {
      current_scene = std::make_unique<TitleScene>(scene_data);
      current_scene->load();
      break;
    }
    case GameLib::SceneType::MENU_SCENE:
    {
      current_scene = std::make_unique<MenuScene>(scene_data);
      current_scene->load();
      break;
    }
    case GameLib::SceneType ::CHAR_SELECT_SCENE:
    {
      current_scene = std::make_unique<CharacterSelect>(scene_data);
      current_scene->load();
      break;
    }
    case GameLib::SceneType::GAME_SCENE:
    {
      current_scene = std::make_unique<GameScene>(scene_data);
      current_scene->load();
      break;
    }
    default:
    {
      break;
    }
  }
}