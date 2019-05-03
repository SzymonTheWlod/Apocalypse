#include "client/Headers/UI/ClickArea.h"

/* Returns true is he passed point is within the area defined by this object */
bool ClickArea::isPointInArea(const Point& point)
{
  return point.x < position.x + width && point.x > position.x &&
         point.y > position.y && point.y < position.y + height;
}