#ifndef PROJECT_SCENEMANAGER_H
#define PROJECT_SCENEMANAGER_H

#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <memory>

#include "Scene.h"
#include "gamelib/Headers/Observer.h"
#include "gamelib/Headers/Point.h"

namespace ASGE
{
class Renderer;
struct EventData;
}

/**
 *  Stores all of the games scenes and handles loading and unloading them
 */
class SceneManager : public GameLib::Observer
{
 public:
  SceneManager() = default;
  ~SceneManager() override = default;

  // Changes the active scene to the passed scene
  bool loadCurrentScene();
  void changeScene(GameLib::SceneType type);
  GameLib::SceneType updateCurrentScene(double delta_time);
  void renderCurrentScene(double delta_time);
  void sceneKeyHandler(ASGE::SharedEventData data);
  void sceneMouseHandler(ASGE::ClickEvent& click);

  void onNotify(GameLib::NetworkPacket& data) override;

 private:
  std::unique_ptr<Scene> current_scene = nullptr;
};

#endif // PROJECT_SCENEMANAGER_H
