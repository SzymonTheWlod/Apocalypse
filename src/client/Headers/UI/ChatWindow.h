#ifndef PROJECT_CHATWINDOW_H
#define PROJECT_CHATWINDOW_H

#include "TextBox.h"
#include "gamelib/Headers/Observer.h"

class ChatWindow
  : public UI
  , public GameLib::Observer
{
 public:
  explicit ChatWindow(Point pos,
                      float _width = 300,
                      float _height = 100,
                      float _font_size = 1,
                      ASGE::Colour _colour = ASGE::COLOURS::BLACK,
                      Point _padding = Point(10, 10));

  void pushMessage(std::string new_message);
  void setBackgroundSprite(const std::string& sprite_texture_path);
  void render(double delta_time) override;
  void onNotify(GameLib::NetworkPacket& data) override;

  void setUsername(std::string& new_username) { username = new_username; };

 private:
  std::string username;
  float font_size = 1;
  ASGE::Colour font_colour = ASGE::COLOURS::BLACK;
  Point padding = Point(10, 10);

  std::vector<std::unique_ptr<TextBox>> messages;
  int max_lines = 0;
  float line_height = 0;
  int line_count = 0;
  Point start_pos = Point(0, 0);

  std::unique_ptr<AnimatedSprite> background_sprite = nullptr;
};

#endif // PROJECT_CHATWINDOW_H
