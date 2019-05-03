#include "gamelib/Headers/Point.h"
#include <math.h>

void Point::set(float new_x, float new_y)
{
  x = new_x;
  y = new_y;
}

bool Point::operator==(Point& point)
{
  float x_dif = fabs(this->x - point.x);
  float y_dif = fabs(this->y - point.y);
  return x_dif < 0.01f && y_dif < 0.01f;
}

Point Point::operator+(const Point& point)
{
  Point return_point;
  return_point.x = this->x + point.x;
  return_point.y = this->y + point.y;
  return return_point;
}

Point Point::operator-(const Point& point)
{
  Point return_point;
  return_point.x = this->x - point.x;
  return_point.y = this->y - point.y;
  return return_point;
}

Point Point::Point::operator/(const float divider)
{
  Point return_point;
  return_point.x = this->x / divider;
  return_point.y = this->y / divider;
  return return_point;
}

Point Point::Point::operator*(const float multiplier)
{
  Point return_point;
  return_point.x = this->x * multiplier;
  return_point.y = this->y * multiplier;
  return return_point;
}

void Point::makeAbsolute()
{
  if (x < 0)
  {
    x *= -1;
  }
  if (y < 0)
  {
    y *= -1;
  }
}

float Point::distanceTo(Point point)
{
  point = point - *this;
  point.makeAbsolute();
  return std::sqrt((point.x * point.x) + (point.y * point.y));
}