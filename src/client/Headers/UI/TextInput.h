#ifndef PROJECT_TEXTINPUT_H
#define PROJECT_TEXTINPUT_H

#include "ClickArea.h"
#include "TextBox.h"
#include "gamelib/Headers/Subject.h"
#include <Engine/InputEvents.h>

class TextInput
  : public TextBox
  , public GameLib::Observer
  , public GameLib::Subject
{
 public:
  explicit TextInput(Point pos,
                     const std::string& placeholder_text,
                     GameLib::EventType e_type = GameLib::EventType::DEFAULT,
                     float _width = 300,
                     float _height = 100,
                     float _font_size = 1,
                     ASGE::Colour colour = ASGE::COLOURS::BLACK,
                     Point _padding = Point(10, 10));

  ~TextInput() override;

  void checkForClick(Point click_location);
  void userInput(ASGE::KeyEvent key_event);
  void doneEditing();
  void onNotify(GameLib::NetworkPacket& data) override;
  void setClearOnSubmit(bool should_clear) { clear_on_submit = should_clear; };

  bool hasFocus() { return has_focus; };

 private:
  GameLib::EventType event_type = GameLib::EventType ::DEFAULT;
  std::string placeholder = "";
  ClickArea click_area;
  bool has_focus = false;
  bool clear_on_submit = false;
};

#endif // PROJECT_TEXTINPUT_H
