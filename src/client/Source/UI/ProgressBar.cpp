#include "client/Headers/UI/ProgressBar.h"
#include "client/Headers/Locator.h"

ProgressBar::ProgressBar(Point pos,
                         float bar_width,
                         float bar_height,
                         int direction,
                         Point _fill_padding) :
  UI(pos),
  fill_padding(_fill_padding)
{
  width = bar_width;
  height = bar_height;
  if (direction == 1)
  {
    horizontal = false;
  }
}

/* Renders the background sprite then the fill sprite on top */
void ProgressBar::render(double delta_time)
{
  if (background_sprite)
  {
    background_sprite->render(delta_time);
  }
  if (fill_sprite)
  {
    fill_sprite->render(delta_time);
  }
}

void ProgressBar::moveTo(Point point)
{
  position = point;
  background_sprite->xPos(point.x);
  background_sprite->yPos(point.y);
  fill_sprite->xPos(point.x);
  fill_sprite->yPos(point.y);
}

/* Adds a background sprite to this object, then scales and rotates it fit the
 * progress bar */
void ProgressBar::addBackgroundSprite(const std::string& sprite_texture_path)
{
  background_sprite =
    std::make_unique<AnimatedSprite>(sprite_texture_path, false, false);
  background_sprite->xPos(position.x);
  background_sprite->yPos(position.y);
  background_sprite->setWidth(width);
  background_sprite->setHeight(height);
}

/* Adds a fill sprite to this object, then scales and rotates it fit the
 * progress bar */
void ProgressBar::addFillSprite(const std::string& sprite_texture_path)
{
  fill_sprite =
    std::make_unique<AnimatedSprite>(sprite_texture_path, false, false);
  fill_sprite->xPos(position.x + (fill_padding.x / 2));
  fill_sprite->yPos(position.y + (fill_padding.y / 2));
  if (horizontal)
  {
    fill_sprite->setWidth((width - fill_padding.x) * bar_progress);
    fill_sprite->setHeight(height - fill_padding.y);
  }
  else
  {
    fill_sprite->setWidth((width - fill_padding.x));
    fill_sprite->setHeight((height - fill_padding.y) * bar_progress);
  }
}

void ProgressBar::addProgress(float delta)
{
  bar_progress += delta;
  if (bar_progress > 1)
  {
    bar_progress = 1;
  }
  else if (bar_progress < 0)
  {
    bar_progress = 0;
  }
  if (horizontal)
  {
    fill_sprite->setWidth((width - fill_padding.x) * bar_progress);
  }
  else
  {
    fill_sprite->setHeight((height - fill_padding.y) * bar_progress);
  }
}

void ProgressBar::setProgress(float amount)
{
  bar_progress = amount;
  if (horizontal)
  {
    fill_sprite->setWidth((width - fill_padding.x) * bar_progress);
  }
  else
  {
    fill_sprite->setHeight((height - fill_padding.y) * bar_progress);
  }
}