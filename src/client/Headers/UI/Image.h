#ifndef PROJECT_IMAGE_H
#define PROJECT_IMAGE_H

#include "UI.h"

// This class is basically just an AnimatedSprite, but it can benefit
// from polymorphism as it is UI
class Image : public UI
{
 public:
  explicit Image(const std::string& texture_path, Point pos);
  ~Image() override = default;

  void render(double delta_time) override;

 private:
  std::unique_ptr<AnimatedSprite> sprite = nullptr;
};

#endif // PROJECT_IMAGE_H
