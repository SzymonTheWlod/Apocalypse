#ifndef PROJECT_POINT_H
#define PROJECT_POINT_H

/**
**   Represents a point in 2d space
*/
struct Point
{
  Point(float px, float py) : x(px), y(py){};
  Point() = default;
  float x = 0;
  float y = 0;

  void set(float new_x, float new_y);
  float distanceTo(Point point);

  bool operator==(Point& point);
  Point operator-(const Point& point);
  Point operator+(const Point& point);
  Point operator/(const float divider);
  Point operator*(const float multiplier);

  void makeAbsolute();
};

#endif // PROJECT_POINT_H
