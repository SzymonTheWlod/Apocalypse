#ifndef PROJECT_BUTTON_H
#define PROJECT_BUTTON_H

#include "ClickArea.h"
#include "TextBox.h"
#include "UI.h"
#include "gamelib/Headers/Resources.h"
#include "gamelib/Headers/Subject.h"
#include <Engine/InputEvents.h>
#include <functional>

class Button
  : public UI
  , public GameLib::Subject
  , public GameLib::Observer
{
 public:
  Button() = default;
  Button(Point pos,
         const std::string& first_texture_path,
         const std::string& second_texture_path,
         GameLib::EventType e_type = GameLib::EventType::DEFAULT,
         float _width = 100,
         float _height = 50);
  ~Button() override;

  void render(double delta_time) override;
  void moveTo(Point point) override;
  void checkForClick(ASGE::ClickEvent click);
  void onNotify(GameLib::NetworkPacket& data) override;
  void setEnabled(bool _enabled) { enabled = _enabled; };
  bool isEnabled() { return enabled; };

  void addText(const std::string& text_string);
  void editText(const std::string& new_string);
  void changeTextColour(const ASGE::Colour& colour);

  void setResources(const Resources& res) { resources = res; };
  void sendResources(bool should_send) { send_resources = should_send; };

 private:
  bool click_down = false;
  bool enabled = true;

  GameLib::EventType event_type = GameLib::EventType::DEFAULT;
  std::unique_ptr<AnimatedSprite> sprite = nullptr;

  std::unique_ptr<TextBox> text = nullptr;

  bool send_resources = false;
  Resources resources;
};

#endif // PROJECT_BUTTON_H
