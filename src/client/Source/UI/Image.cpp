#include "client/Headers/UI/Image.h"

Image::Image(const std::string& texture_path, Point pos) :
  UI(pos), sprite(std::make_unique<AnimatedSprite>(texture_path, false, false))
{
  sprite->xPos(pos.x);
  sprite->yPos(pos.y);
}

void Image::render(double delta_time)
{
  if (visible)
  {
    sprite->render(delta_time);
  }
}