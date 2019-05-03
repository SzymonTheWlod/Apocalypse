#ifndef PROJECT_TITLESCENE_H
#define PROJECT_TITLESCENE_H

#include "../AnimatedSprite.h"
#include "Scene.h"

namespace ASGE
{
class Renderer;
}

/**
**   The first scene of the game, displays logos
*/
class TitleScene : public Scene
{
 public:
  TitleScene() = default;
  explicit TitleScene(PersistentClientData& data) : Scene(data){};
  ~TitleScene() override = default;
  bool load() override;
  GameLib::SceneType update(double delta_time) override;
  void render(double delta_time) override;
  void keyHandler(const ASGE::SharedEventData data) override;
  void mouseHandler(ASGE::ClickEvent& click) override;

 private:
  // All sprites for this scene
  std::unique_ptr<AnimatedSprite> title = nullptr;
  bool fading = false;
  double time_waited = 0;
};

#endif // PROJECT_TITLESCENE_H
