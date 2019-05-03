#ifndef PROJECT_GAMERENDERER_H
#define PROJECT_GAMERENDERER_H

#include "gamelib/Headers/Point.h"
#include <Engine/Renderer.h>

class GameRenderer
{
 public:
  explicit GameRenderer(ASGE::Renderer* rend);

  ASGE::Sprite* createRawSprite() { return renderer->createRawSprite(); };
  void setClearColour(ASGE::Colour colour)
  {
    renderer->setClearColour(colour);
  };

  void renderSpriteToWorld(ASGE::Sprite* sprite);
  void renderSpriteToScreen(ASGE::Sprite* sprite);

  void renderTextToWorld(std::string& text,
                         Point pos,
                         float& scale,
                         ASGE::Colour& colour);
  void renderTextToScreen(std::string& text,
                          Point pos,
                          float& scale,
                          ASGE::Colour& colour);

  Point getCameraPosition() { return position; };
  Point displayedToSimulatedWorld(Point point);
  void moveCamera(Point amount);
  void zoomCamera(float zoom_amount, Point zoom_focus);

  void setCameraLimits(const Point& lower, const Point& upper);

  void cycleWindowMode();
  ASGE::Renderer::WindowMode getWindowMode()
  {
    return renderer->getWindowMode();
  };

  void resetCamera();

 private:
  Point position = Point(0, 0);
  Point upper_limit = Point(0, 0);
  Point lower_limit = Point(0, 0);

  float zoom = 1;
  float max_zoom = 2;
  float min_zoom = 0.5f;
  ASGE::Renderer* renderer = nullptr;
};

#endif // PROJECT_GAMERENDERER_H
