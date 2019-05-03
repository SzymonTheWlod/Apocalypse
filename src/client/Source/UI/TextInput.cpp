#include "client/Headers/UI/TextInput.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Locator.h"
#include <Engine/Mouse.h>

TextInput::TextInput(Point pos,
                     const std::string& placeholder_text,
                     GameLib::EventType e_type,
                     float _width,
                     float _height,
                     float _font_size,
                     ASGE::Colour colour,
                     Point _padding) :
  TextBox(pos, placeholder_text, _width, _height, _font_size, colour, _padding),
  event_type(e_type), placeholder(placeholder_text),
  click_area(ClickArea(pos, _width, _height))
{
  Locator::instance->getInputManager()->addObserver(
    this, GameLib::EventType::KEY_PRESSED_ANY);
  Locator::instance->getInputManager()->addObserver(
    this, GameLib::EventType::MOUSE_CLICKED);
}

TextInput::~TextInput()
{
  Locator::instance->getInputManager()->removeObserver(
    this, GameLib::EventType::KEY_PRESSED_ANY);
  Locator::instance->getInputManager()->removeObserver(
    this, GameLib::EventType::MOUSE_CLICKED);
}

void TextInput::checkForClick(Point click_location)
{
  if (click_area.isPointInArea(click_location))
  {
    has_focus = true;
    // Clear the text if its the placeholder
    if (displayed_text == placeholder)
    {
      displayed_text = "";
    }
  }
  else if (has_focus)
  {
    has_focus = false;
    if (!clear_on_submit)
    {
      doneEditing();
    }
    if (displayed_text.empty())
    {
      displayed_text = placeholder;
    }
  }
}

void TextInput::userInput(ASGE::KeyEvent key_event)
{
  if (has_focus && key_event.action == ASGE::KEYS::KEY_PRESSED)
  {
    if (key_event.key == ASGE::KEYS::KEY_ENTER)
    {
      doneEditing();
    }
    else if (key_event.key == ASGE::KEYS::KEY_BACKSPACE &&
             displayed_text.length() > 0)
    {
      displayed_text.resize(displayed_text.length() - 1);
    }
    else
    {
      displayed_text += static_cast<char>(key_event.key);
    }
  }
}

void TextInput::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::KEY_PRESSED_ANY)
  {
    ASGE::KeyEvent key_event;
    data >> key_event.action;
    data >> key_event.key;
    data >> key_event.mods;
    data >> key_event.scancode;

    if (key_event.action == ASGE::MOUSE::BUTTON_PRESSED)
    {
      userInput(key_event);
    }
  }
  else if (data.getType() == GameLib::EventType::MOUSE_CLICKED)
  {
    ASGE::ClickEvent click_event;
    data >> click_event.action;
    data >> click_event.button;
    data >> click_event.mods;
    data >> click_event.xpos;
    data >> click_event.ypos;
    checkForClick(Point(static_cast<float>(click_event.xpos),
                        static_cast<float>(click_event.ypos)));
  }
}

void TextInput::doneEditing()
{
  GameLib::NetworkPacket packet(event_type);
  packet << displayed_text;

  notify(packet.getType(), packet);
  if (clear_on_submit)
  {
    displayed_text = "";
  }
}