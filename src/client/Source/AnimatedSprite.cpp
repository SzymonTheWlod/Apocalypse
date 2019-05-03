#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"
#include <Engine/Renderer.h>

/**
 *   @brief   Creates an AnimatedSprite with set size
 *   @details num_of_sprites is the number of contained ASGE::Sprite's
 */
AnimatedSprite::AnimatedSprite(const std::string& texture_path,
                               bool should_flipbook,
                               bool render_to_world) :
  auto_anim(should_flipbook),
  world_render(render_to_world)
{
  addSprite(texture_path);
}

AnimatedSprite::AnimatedSprite(bool should_flipbook, bool render_to_world) :
  auto_anim(should_flipbook), world_render(render_to_world)
{
}

AnimatedSprite::~AnimatedSprite()
{
  // Signal the lerp thread to exit
  lerp_mutex.lock();
  lerping = false;
  lerp_mutex.unlock();
  // Wait for the thread to exit
  std::lock_guard<std::mutex> guard(delete_mutex);
}

/**
 *   @brief   Adds a sprite to the object
 *   @details Adds a new ASGE::Sprite to this AnimatedSprite,
 *            also intializes the position, height and width
 *            if this is the first sprite to be added
 *   @param	 Adress of new sprite
 */
void AnimatedSprite::addSprite(const std::string& texture_path)
{
  ASGE::Sprite* sprite = Locator::instance->getRenderer()->createRawSprite();
  sprite->loadTexture(texture_path);
  my_sprites.push_back(std::unique_ptr<ASGE::Sprite>(sprite));
  if (my_sprites.size() - 1 == 0)
  {
    sprite_height = my_sprites.back()->height();
    sprite_width = my_sprites.back()->width();
  }
  my_sprites.back()->scale(width_scale);
}

/**
 *   @brief   Get the x position of sprite
 *   @details Gets the x position of ASGE::Sprite,
 *			 this should be in the displayed world position
 *   @return  The x position of sprite
 */
float AnimatedSprite::xPos()
{
  return my_sprites[0]->xPos();
}

/**
 *   @brief   Get the y position of sprite
 *   @details Gets the y position of ASGE::Sprite,
 *			 this should be in the displayed world position
 *   @return  The y position of sprite
 */
float AnimatedSprite::yPos()
{
  return my_sprites[0]->yPos();
}

/**
 *   @brief   Sets the x position of sprite(s)
 *   @details Sets the x position of all attached sprites
 *   @param   The new x position for the sprite(s)
 */
void AnimatedSprite::xPos(float new_x)
{
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->xPos(new_x * width_scale);
  }
}

/* Adds an amount to the current x_position contained sprites */
void AnimatedSprite::addX(float x_amount)
{
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->xPos(my_sprites[i]->xPos() + x_amount);
  }
}

/* Adds an amount to the current y_position contained sprites */
void AnimatedSprite::addY(float y_amount)
{
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->yPos(my_sprites[i]->yPos() + y_amount);
  }
}

/**
 *   @brief   Sets the y position of sprite(s)
 *   @details Sets the y position of all attached sprites
 *   @param  The new y position for the sprite(s)
 */
void AnimatedSprite::yPos(float new_y)
{
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->yPos(new_y * width_scale);
  }
}

/**
 *   @brief   Gets the scale of sprite(s)
 *   @details Gets the scale of the AnimatedSprite, this is the scale
 *            of the displayed world
 *   @return  The sprites scale
 */
float AnimatedSprite::scale()
{
  return scale_factor;
}

/**
 *   @brief   Get the width of sprite
 *   @details Gets the width of the AnimatedSprite,
 *			 read only - use scale to alter size
 *   @return  The width of sprite
 */
float AnimatedSprite::width()
{
  return sprite_width * scale_factor;
}

/**
 *   @brief   Get the height of sprite
 *   @details Gets the height of the AnimatedSprite,
 *			 read only - use scale to alter size
 *   @return  The height of sprite
 */
float AnimatedSprite::height()
{
  return sprite_height * scale_factor;
}

/**
 *   @brief   Sets the scale of sprite(s)
 *   @details Sets the scale of AnimatedSprite,
 *			 this scale is automatically applied and
 *			 scaled to the scale of all contained sprites
 *   @param   The new scale for the sprite(s)
 */
void AnimatedSprite::scale(float scale_amount)
{
  scale_factor = scale_amount * width_scale;
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->scale(scale_factor);
  }
}

/**
 *   @brief   Sets the width of sprite(s)
 *   @details Sets the width of AnimatedSprite,
 *			 this width is automatically applied
 *			 to the width of all contained sprites
 *   @param   The new width for the sprite(s)
 */
void AnimatedSprite::setWidth(float new_width)
{
  sprite_width = new_width;
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->width(sprite_width * scale_factor);
  }
}

/**
 *   @brief   Sets the height of sprite(s)
 *   @details Sets the height of AnimatedSprite,
 *			 this height is automatically applied
 *			 to the height of all contained sprites
 *   @param   The new height for the sprite(s)
 */
void AnimatedSprite::setHeight(float new_height)
{
  sprite_height = new_height;
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->height(sprite_height * scale_factor);
  }
}

/* Sets the rotation of this sprite to the passed argument (in radians) */
void AnimatedSprite::setRotation(float new_rotation)
{
  rotation = new_rotation;
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->rotationInRadians(rotation);
  }
}

/* Sets the current index for which sprite is returned */
void AnimatedSprite::setCurrentSprite(size_t new_sprite_index)
{
  if (new_sprite_index >= my_sprites.size())
  {
    current_sprite = my_sprites.size() - 1;
  }
  else
  {
    current_sprite = new_sprite_index;
  }
}

void AnimatedSprite::setColour(ASGE::Colour col)
{
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->colour(col);
  }
}

void AnimatedSprite::fadeToColour(ASGE::Colour colour, double time_to_fade)
{
  start_colour = my_sprites[0]->colour();
  colour_diff.r = colour.r - start_colour.r;
  colour_diff.b = colour.b - start_colour.b;
  colour_diff.g = colour.g - start_colour.g;
  col_fade_duration = time_to_fade;
  col_fade_elapsed = 0;
}

void AnimatedSprite::fade(float opacity, double time_to_fade)
{
  target_opacity = opacity;
  start_opacity = my_sprites[0]->opacity();
  fade_duration = time_to_fade;
}

/**
 *   @brief   Sets the speed at which the sprite is animated
 *   @details Sets the time between animation frames used in returnNextSprite
 *           to calculate when to switch to the next frame
 *   @param   n_time_btwn_frames how quickly you want
 *           the sprite to animate
 */
void AnimatedSprite::timeBetweenFrames(double n_time_btwn_frames)
{
  time_between_frames = n_time_btwn_frames;
}

/**
 *   @brief   Gets the next frame of the sprite
 *   @details Sets the current step of animation to next step or the first
 *			 step if the animation is over if enough time has passed
 *   @param   delta_time is time since last call
 *   @return  The next sprite of the animation
 */
void AnimatedSprite::render(double delta_time)
{
  if (lerping)
  {
    addLerpToPos();
  }

  if (auto_anim)
  {
    time_passed += delta_time;
    if (time_passed > time_between_frames)
    {
      current_sprite++;
      time_passed -= time_between_frames;
    }

    if (current_sprite == my_sprites.size())
    {
      current_sprite = 0;
    }
  }

  if (target_opacity != my_sprites[0]->opacity())
  {
    updateOpacity(delta_time);
  }

  if (col_fade_elapsed != col_fade_duration)
  {
    updateColour(delta_time);
  }

  if (my_sprites[0]->opacity() == 0)
  {
    return;
  }

  if (world_render)
  {
    Locator::instance->getRenderer()->renderSpriteToWorld(
      my_sprites[current_sprite].get());
  }
  else
  {
    Locator::instance->getRenderer()->renderSpriteToScreen(
      my_sprites[current_sprite].get());
  }
}

void AnimatedSprite::updateColour(double delta_time)
{
  col_fade_elapsed += delta_time;
  if (col_fade_elapsed > col_fade_duration)
  {
    col_fade_elapsed = col_fade_duration;
  }

  auto colour_fade_percent =
    static_cast<float>(col_fade_elapsed / col_fade_duration);

  ASGE::Colour new_col = ASGE::COLOURS::WHITE;
  new_col.r = start_colour.r + (colour_diff.r * colour_fade_percent);
  new_col.g = start_colour.b + (colour_diff.b * colour_fade_percent);
  new_col.b = start_colour.g + (colour_diff.g * colour_fade_percent);
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->colour(new_col);
  }
}

void AnimatedSprite::updateOpacity(double delta_time)
{
  float new_opacity =
    my_sprites[0]->opacity() + ((target_opacity - start_opacity) *
                                static_cast<float>(delta_time / fade_duration));
  if (target_opacity > start_opacity && new_opacity > target_opacity)
  {
    new_opacity = target_opacity;
  }
  else if (target_opacity < start_opacity && new_opacity < target_opacity)
  {
    new_opacity = target_opacity;
  }
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->opacity(new_opacity);
  }
}

void AnimatedSprite::lerpTo(float x_pos, float y_pos, double duration)
{
  if (lerping)
  {
    return;
  }
  lerp_x = my_sprites[0]->xPos();
  lerp_y = my_sprites[0]->yPos();
  lerping = true;
  // This mutex prevents this object from being destroyed until all threads have
  // exited
  x_pos -= lerp_x;
  y_pos -= lerp_y;
  delete_mutex.lock();
  lerp_thread =
    std::thread(&AnimatedSprite::lerpThread, this, x_pos, y_pos, duration);
  lerp_thread.detach();
}

void AnimatedSprite::lerpThread(float x_pos, float y_pos, double duration)
{
  double durat = duration;
  if (durat < 10)
  {
    durat = 10;
  }
  float x_step = x_pos / (static_cast<float>(durat) / 10);
  float y_step = y_pos / (static_cast<float>(durat) / 10);

  double time_elapsed = 0;

  while (time_elapsed < durat)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    time_elapsed += 10;
    std::lock_guard<std::mutex> guard(lerp_mutex);
    // This is set to false if the main thread is trying to delete this object
    if (!lerping)
    {
      delete_mutex.unlock();
      return;
    }
    lerp_x += x_step;
    lerp_y += y_step;
  }
  lerping = false;
  delete_mutex.unlock();
}

void AnimatedSprite::addLerpToPos()
{
  // std::lock_guard<std::mutex> guard(lerp_mutex);
  for (size_t i = 0; i < my_sprites.size(); i++)
  {
    my_sprites[i]->xPos(lerp_x);
    my_sprites[i]->yPos(lerp_y);
  }
}

float AnimatedSprite::width_scale = 1;