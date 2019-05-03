#ifndef PROJECT_ANIMATEDSPRITE_H
#define PROJECT_ANIMATEDSPRITE_H

#include <Engine/Sprite.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

/**
 *  Component which contains an array of sprites which can be animated with its
 *  methods. Also contains other useful functionality such as scaling
 *  Animated sprites have their position and scale set to the base game
 *  resolution, this is referred to as the simulated world
 *  When retrieving the ASGE::Sprite from the AnimatedSprite it will give the
 *  position and scale based on what the set screen resolution is, this is
 *  referred to as the displayed world. No manual conversion is necessary,
 *  just set the static field width_scale at run time
 */
class AnimatedSprite
{
 public:
  explicit AnimatedSprite(const std::string& texture_path,
                          bool should_flipbook = false,
                          bool render_to_world = true);
  AnimatedSprite(bool should_flipbook, bool render_to_world);
  ~AnimatedSprite();

  AnimatedSprite(const AnimatedSprite&) = delete;
  AnimatedSprite& operator=(const AnimatedSprite&) = delete;

  void xPos(float new_x);
  float xPos();
  void addX(float x_amount);
  void yPos(float new_y);
  float yPos();
  void addY(float y_amount);
  float scale();
  void scale(float new_scale);
  float width();
  float height();
  void setWidth(float new_width);
  void setHeight(float new_height);
  void setRotation(float new_rotation);
  void timeBetweenFrames(double n_time_btwn_frames);
  void setCurrentSprite(size_t new_sprite_index);

  void setColour(ASGE::Colour col);

  void addSprite(const std::string& texture_path);
  void fadeToColour(ASGE::Colour colour, double time_to_fade);
  void render(double delta_time);

  void fade(float opacity, double time_to_fade);
  float getOpacity() { return my_sprites[0]->opacity(); };

  void lerpTo(float x_pos, float y_pos, double duration);
  bool isLerping() { return lerping; };
  static float width_scale; /**< The scale difference between actual and base
                               width */

 private:
  void updateOpacity(double delta_time);
  void updateColour(double delta_time);

  void lerpThread(float x_pos, float y_pos, double duration);
  void addLerpToPos();

  float lerp_x = 0;
  float lerp_y = 0;
  std::atomic<bool> lerping = false;
  std::mutex lerp_mutex;
  std::mutex delete_mutex;
  std::thread lerp_thread;

  float sprite_width = 50;
  float sprite_height = 50;
  float scale_factor = 1;
  float rotation = 0;
  double time_passed = 0;
  double time_between_frames = 0;
  bool auto_anim = false;
  size_t current_sprite = 0;
  bool world_render = true;

  ASGE::Colour start_colour = ASGE::COLOURS::WHITE;
  ASGE::Colour colour_diff = ASGE::COLOURS::WHITE;
  double col_fade_duration = 0;
  double col_fade_elapsed = 0;

  float target_opacity = 1;
  float start_opacity = 1;
  double fade_duration = 0;

  std::vector<std::unique_ptr<ASGE::Sprite>> my_sprites;
};

#endif // PROJECT_ANIMATEDSPRITE_H
