#include "client/Headers/InputManager.h"
#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"

void InputManager::processKeyData(const ASGE::KeyEvent* data)
{
  GameLib::NetworkPacket packet(GameLib::EventType ::KEY_PRESSED_ANY);
  packet << data->action << data->key << data->mods << data->scancode;
  notify(packet.getType(), packet);
}

void InputManager::processClickData(const ASGE::ClickEvent* data)
{
  GameLib::NetworkPacket packet(GameLib::EventType::MOUSE_CLICKED);
  packet << data->action << data->button << data->mods << data->xpos
         << data->ypos;
  notify(packet.getType(), packet);
}

void InputManager::processScrollData(const ASGE::ScrollEvent* data)
{
  GameLib::NetworkPacket packet(GameLib::EventType::MOUSE_SCROLLED);
  packet << data->yoffset << data->xoffset;
  notify(packet.getType(), packet);
}

Point InputManager::getWorldMousePosition()
{
  double x_pos = 0;
  double y_pos = 0;
  input->getCursorPos(x_pos, y_pos);
  Point pos = Point(static_cast<float>(x_pos), static_cast<float>(y_pos));
  return Locator::instance->getRenderer()->displayedToSimulatedWorld(pos);
}

Point InputManager::getScreenMousePosition()
{
  double x_pos = 0;
  double y_pos = 0;
  input->getCursorPos(x_pos, y_pos);
  return Point(static_cast<float>(x_pos),
               static_cast<float>(y_pos) / AnimatedSprite::width_scale);
}
