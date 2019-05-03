#include "client/Headers/UI/ChatWindow.h"
#include "client/Headers/Network/NetworkManager.h"
#include <client/Headers/Locator.h>
#include <cmath>

ChatWindow::ChatWindow(Point pos,
                       float _width,
                       float _height,
                       float _font_size,
                       ASGE::Colour colour,
                       Point _padding) :
  UI(pos),
  font_size(_font_size * AnimatedSprite::width_scale), font_colour(colour),
  padding(_padding)
{
  width = _width;
  height = _height;
  max_lines = static_cast<int>(std::floor(_height / (font_size * 19)));
  line_height = font_size * 19;
  start_pos =
    position + Point(0, line_height * (static_cast<float>(max_lines - 1)) + 10);
}

void ChatWindow::pushMessage(std::string new_message)
{
  messages.emplace_back(std::make_unique<TextBox>(start_pos,
                                                  new_message,
                                                  width,
                                                  line_height,
                                                  font_size,
                                                  font_colour,
                                                  padding));
  line_count += (messages.end() - 1)->get()->getNumLines();

  (messages.end() - 1)
    ->get()
    ->moveBy(Point(
      0, static_cast<float>((messages.end() - 1)->get()->getNumLines() * -20)));
  // Remove messages till the new one fits
  while (line_count > max_lines)
  {
    line_count -= messages.begin()->get()->getNumLines();
    messages.erase(messages.begin());
  }

  // Move all other existing messages up
  for (unsigned long long i = 0; i < messages.size() - 1; i++)
  {
    messages[i]->moveBy(
      Point(0,
            -line_height *
              static_cast<float>((messages.end() - 1)->get()->getNumLines())));
  }
}

/* Sets the sprite that will be rendered behind the text */
void ChatWindow::setBackgroundSprite(const std::string& sprite_texture_path)
{
  background_sprite =
    std::make_unique<AnimatedSprite>(sprite_texture_path, false, false);
  background_sprite->setWidth(width);
  background_sprite->setHeight(height);
  background_sprite->xPos(position.x);
  background_sprite->yPos(position.y);
}

void ChatWindow::render(double delta_time)
{
  if (visible)
  {
    if (background_sprite)
    {
      background_sprite->render(delta_time);
    }
    for (unsigned long long i = 0; i < messages.size(); i++)
    {
      messages[i]->render(delta_time);
    }
  }
}

void ChatWindow::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::INPUT_SUBMITTED)
  {
    std::string msg;
    data >> msg;

    if (msg.empty())
    {
      return;
    }

    // Add the username to the message, then send it to the server
    std::string message = username + ": " + msg;
    GameLib::NetworkPacket packet(GameLib::EventType::CHAT_MESSAGE);
    packet << message;
    Locator::instance->getNetworkManager()->sendDataToServer(packet);
    pushMessage(message);
  }
}