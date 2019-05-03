#ifndef PROJECT_UI_H
#define PROJECT_UI_H

#include "ClickArea.h"
#include "client/Headers/AnimatedSprite.h"
#include "gamelib/Headers/Point.h"
#include <Engine/Renderer.h>

/* Virtual base class for all UI elements */
class UI
{
 public:
  UI() = default;
  explicit UI(Point pos) : position(pos){};
  virtual ~UI() = default;

  virtual void render(double delta_time) = 0;

  virtual void moveTo(Point point) { position = point; };
  void moveBy(Point point);

  Point getPosition() { return position; };
  float getWidth() { return width; };
  float getHeight() { return height; };

  void setVisible(bool isVisible) { visible = isVisible; };
  bool doesClickHit(const Point& click)
  {
    return click_area.isPointInArea(click);
  };

 protected:
  ClickArea click_area;
  bool visible = true;
  Point position = Point(0, 0);
  float width = 0;
  float height = 0;
};

#endif // PROJECT_UI_H
