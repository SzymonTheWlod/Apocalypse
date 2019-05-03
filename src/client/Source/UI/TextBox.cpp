#include "client/Headers/UI/TextBox.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"

TextBox::TextBox(Point pos,
                 const std::string& text,
                 float _width,
                 float _height,
                 float _font_size,
                 ASGE::Colour colour,
                 Point _padding) :
  UI(pos),
  displayed_text(text), font_size(_font_size * AnimatedSprite::width_scale),
  font_colour(colour), padding(_padding)
{
  width = _width;
  height = _height;
  wrapText();
}

/* Renders this text to screen with its background (if any) */
void TextBox::render(double delta_time)
{
  if (background_sprite)
  {
    background_sprite->render(delta_time);
  }
  Locator::instance->getRenderer()->renderTextToScreen(
    displayed_text,
    Point(position.x + padding.x, position.y + padding.y + (font_size * 20)) *
      AnimatedSprite::width_scale,
    font_size,
    font_colour);
}

/* Sets the sprite that will be rendered behind the text */
void TextBox::setBackgroundSprite(const std::string& sprite_texture_path)
{
  background_sprite =
    std::make_unique<AnimatedSprite>(sprite_texture_path, false, false);
  background_sprite->setWidth(width);
  background_sprite->setHeight(height);
  background_sprite->xPos(position.x);
  background_sprite->yPos(position.y);
}

/* Replaces the current text with the passed argument */
void TextBox::setText(const std::string& new_text)
{
  displayed_text = new_text;
  wrapText();
}

/* Horizontally wraps text to stay within the width of this TextBos */
void TextBox::wrapText()
{
  // Calculate the max number of characters per line, this is only roughly
  // accurate unfortunately Also need to remove the width_scale since all other
  // calculations are done in simulated world space
  auto char_per_line = static_cast<unsigned long long>(
    (width - (2 * padding.x)) / (font_size / AnimatedSprite::width_scale));
  // Divide by some number, may vary by font, unsure on how to get this part
  // exact
  char_per_line /= 11;
  unsigned long long str_index = 0;

  // While there are characters left to process
  while (str_index < displayed_text.size())
  {
    // Check if any newlines already exist
    bool newline_exists = false;
    for (unsigned long long i = str_index; i < str_index + char_per_line; i++)
    {
      if (displayed_text[i] == '\n')
      {
        num_lines++;
        newline_exists = true;
        str_index = i + 1;
        break;
      }
    }
    if (!newline_exists)
    {
      num_lines++;
      if (backtrack(str_index, char_per_line))
      {
        return;
      }
    }
  }
}

bool TextBox::backtrack(unsigned long long& _str_index,
                        unsigned long long& _char_per_line)
{
  _str_index += _char_per_line;
  // If there is a full line left
  if (_str_index >= displayed_text.length())
  {
    return false;
  }
  // Backtrack to the last space
  while (displayed_text[_str_index] != ' ')
  {
    // If the index gets to 0, this is the best we can do so finish here
    if (_str_index == 0)
    {
      return true;
    }
    _str_index--;
  }
  // Replace that space with a newline
  displayed_text[_str_index] = '\n';
  _str_index++;
  return false;
}

void TextBox::changeColour(const ASGE::Colour& colour)
{
  font_colour = colour;
}
