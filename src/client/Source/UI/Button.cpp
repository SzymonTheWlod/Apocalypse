#include "client/Headers/UI/Button.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Locator.h"
#include <Engine/Mouse.h>

Button::Button(Point pos,
               const std::string& first_texture_path,
               const std::string& second_texture_path,
               GameLib::EventType e_type,
               float _width,
               float _height) :
  UI(pos),
  event_type(e_type)
{
  click_area = ClickArea(pos, _width, _height);
  width = _width;
  height = _height;
  sprite = std::make_unique<AnimatedSprite>(first_texture_path, false, false);
  sprite->addSprite(second_texture_path);
  sprite->setWidth(width);
  sprite->setHeight(height);
  sprite->xPos(pos.x);
  sprite->yPos(pos.y);

  Locator::instance->getInputManager()->addObserver(
    this, GameLib::EventType::MOUSE_CLICKED);
}

Button::~Button()
{
  Locator::instance->getInputManager()->removeObserver(
    this, GameLib::EventType::MOUSE_CLICKED);
}

void Button::render(double delta_time)
{
  if (visible)
  {
    sprite->render(delta_time);
    if (text)
    {
      text->render(delta_time);
    }
  }
}

void Button::moveTo(Point point)
{
  position = point;
  sprite->xPos(point.x);
  sprite->yPos(point.y);
  click_area.setPosition(point);
}

/* Checks to see if a given point is within this objects click area and updates
 * accordingly */
void Button::checkForClick(ASGE::ClickEvent click)
{
  if (!enabled)
  {
    return;
  }

  if (click_area.isPointInArea(
        Point(static_cast<float>(click.xpos), static_cast<float>(click.ypos))))
  {
    if (click.action == ASGE::MOUSE::BUTTON_PRESSED)
    {
      sprite->setCurrentSprite(1);
      click_down = true;
    }
    else if (click.action == ASGE::MOUSE::BUTTON_RELEASED && click_down)
    {
      sprite->setCurrentSprite(0);
      GameLib::NetworkPacket packet(event_type);
      if (send_resources)
      {
        packet << resources;
      }
      notify(event_type, packet);
      click_down = false;
    }
  }
  else
  {
    sprite->setCurrentSprite(0);
    click_down = false;
  }
}

void Button::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::MOUSE_CLICKED)
  {
    ASGE::ClickEvent click_event;
    data >> click_event.action;
    data >> click_event.button;
    data >> click_event.mods;
    data >> click_event.xpos;
    data >> click_event.ypos;

    checkForClick(click_event);
  }
}

void Button::addText(const std::string& text_string)
{
  text = std::make_unique<TextBox>(position, text_string, width, height);
}

void Button::editText(const std::string& new_string)
{
  text->setText(new_string);
}

void Button::changeTextColour(const ASGE::Colour& colour)
{
  text->changeColour(colour);
}