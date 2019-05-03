#ifndef PROJECT_PROGRESSBAR_H
#define PROJECT_PROGRESSBAR_H

#include "UI.h"
#include "gamelib/Headers/Constants.h"

/* A self contained progress bar that once set only needs render to be called
 * and progress to be updated */
class ProgressBar : public UI
{
 public:
  ProgressBar() = default;
  explicit ProgressBar(Point pos,
                       float bar_width = 100,
                       float bar_height = 20,
                       int direction = GameLib::Rotation::LEFT_TO_RIGHT,
                       Point _fill_padding = Point(5, 5));
  ~ProgressBar() override = default;

  void render(double delta_time) override;
  void moveTo(Point point) override;

  void addBackgroundSprite(const std::string& sprite_texture_path);
  void addFillSprite(const std::string& sprite_texture_path);

  void addProgress(float delta);
  void setProgress(float amount);
  float getProgress() { return bar_progress; };

 private:
  float bar_progress = 0; // Represents a percentage between 0 and 1
  Point fill_padding = Point(0, 0);
  bool horizontal = true;

  std::unique_ptr<AnimatedSprite> background_sprite = nullptr;
  std::unique_ptr<AnimatedSprite> fill_sprite = nullptr;
};

#endif // PROJECT_PROGRESSBAR_H
