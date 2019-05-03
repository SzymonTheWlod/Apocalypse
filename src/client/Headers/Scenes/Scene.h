#ifndef PROJECT_CLIENTSCENE_H
#define PROJECT_CLIENTSCENE_H

#include "client/Headers/Scenes/PersistentClientData.h"
#include "gamelib/Headers/Constants.h"
#include "gamelib/Headers/Point.h"
#include <Engine/Colours.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <memory>

namespace ASGE
{
class Renderer;
struct EventData;
}

/**
 *  Holds all of the data and update code for a scene
 */
class Scene
{
 public:
  Scene() = default;
  explicit Scene(PersistentClientData& data) : scene_data(data){};
  virtual ~Scene() = default;
  virtual bool load() = 0;
  virtual GameLib::SceneType update(double delta_time) = 0;
  virtual void render(double delta_time) = 0;
  virtual void keyHandler(ASGE::SharedEventData data) = 0;
  virtual void mouseHandler(ASGE::ClickEvent& click) = 0;

  GameLib::SceneType next_scene = GameLib::SceneType ::NO_CHANGE;

  PersistentClientData& getPersistentData() { return scene_data; };

 protected:
  PersistentClientData scene_data;
};

#endif // PROJECT_CLIENTSCENE_H
