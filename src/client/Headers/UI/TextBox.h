#ifndef PROJECT_TEXTBOX_H
#define PROJECT_TEXTBOX_H

#include "UI.h"

/* A self contained text box that once set only needs render to be called */
class TextBox : public UI
{
 public:
  TextBox() = default;
  explicit TextBox(Point pos,
                   const std::string& text,
                   float _width = 300,
                   float _height = 100,
                   float _font_size = 1,
                   ASGE::Colour colour = ASGE::COLOURS::BLACK,
                   Point _padding = Point(10, 10));
  ~TextBox() override = default;

  void render(double delta_time) override;
  void setBackgroundSprite(const std::string& sprite_texture_path);
  void setText(const std::string& new_text);
  int getNumLines() { return num_lines; };

  void changeColour(const ASGE::Colour& colour);

 protected:
  void wrapText();
  bool
  backtrack(unsigned long long& _str_index, unsigned long long& _char_per_line);
  std::string displayed_text = "";

 private:
  float font_size = 0;
  int num_lines = 1;
  ASGE::Colour font_colour = ASGE::COLOURS::BLACK;
  Point padding = Point(10, 10); // Offset applied before rendering the text

  std::unique_ptr<AnimatedSprite> background_sprite = nullptr;
};

#endif // PROJECT_TEXTBOX_H
