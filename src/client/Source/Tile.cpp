#include <Engine/Mouse.h>
#include <client/Headers/Tile.h>

#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"
#include "client/Headers/Tile.h"
#include "gamelib/Headers/Printer.h"

using json = nlohmann::json;

Tile::Tile(GameLib::TileType& type,
           float& size,
           size_t index_in_vector,
           const json& config) :
  index(index_in_vector)
{
  building_type = type;
  generateTileTypes(type, size, config);
  GameLib::Printer() << static_cast<int>(type);

  click.setHeight(tile_sprite->height());
  click.setWidth(tile_sprite->width());

  if (name == "Grass" || name == "Town Hall")
  {
    health = 1000000;
    max_health = 1000000;
  }
}

void Tile::generateTileTypes(GameLib::TileType& type,
                             float& size,
                             const json& config)
{
  tile_sprite = std::make_unique<AnimatedSprite>(false, true);

  std::string as_string = std::to_string(static_cast<int>(type));

  name = config[as_string]["name"];
  tile_sprite->addSprite(config[as_string]["file_path"]);

  mask_sprite = std::make_unique<AnimatedSprite>("data/default_black.png");
  mask_sprite->setWidth(size);
  mask_sprite->setHeight(size);
  highlight_sprite = std::make_unique<AnimatedSprite>("data/"
                                                      "tile_highlight.png");
  highlight_sprite->setWidth(size);
  highlight_sprite->setHeight(size);

  tile_sprite->addSprite(config["-1"]["file_path"]);
  tile_sprite->setWidth(size);
  tile_sprite->setHeight(size);
}

void Tile::render(double delta_time)
{
  tile_sprite->render(delta_time);
  mask_sprite->render(delta_time);
  if (highlighted && isRevealed())
  {
    highlight_sprite->render(delta_time);
  }
}

bool Tile::checkForClick(Point pos)
{
  return click.isPointInArea(pos);
}

std::string& Tile::getName()
{
  return name;
}

float& Tile::getHealth()
{
  return health;
}

void Tile::setPosition(Point pos)
{
  position = pos;
  tile_sprite->xPos(pos.x);
  tile_sprite->yPos(pos.y);
  mask_sprite->xPos(pos.x);
  mask_sprite->yPos(pos.y);
  highlight_sprite->xPos(pos.x);
  highlight_sprite->yPos(pos.y);
  click.setPosition(pos);
}

// Finds the offset position to render a character depending on the amount of
// characters on this tile and how many have already been rendered on this tile
Point Tile::getNextRenderPos()
{
  float size = tile_sprite->width();
  Point point;
  switch (characters_on_tile)
  {
    case 1:
    {
      point = position + Point(size / 4, size / 4);
      break;
    }
    case 2:
    {
      point =
        position + Point((size / 2) * static_cast<float>(render_pos), size / 4);
      break;
    }
    case 3:
    {
      if (render_pos == 0)
      {
        point = position + Point(size / 4, 0);
      }
      else
      {
        point =
          position +
          Point((size / 2) * static_cast<float>(render_pos - 1), size / 2);
      }
      break;
    }
    case 4:
    {
      if (render_pos < 2)
      {
        point =
          position + Point((size / 2) * static_cast<float>(render_pos), 0);
      }
      else
      {
        point =
          position +
          Point((size / 2) * (static_cast<float>(render_pos) - 2), size / 2);
      }
      break;
    }
    default:
    {
      point = position; // Shouldn't reach here
      break;
    }
  }
  render_pos++;
  return point;
}

void Tile::setFade(const float& fade)
{
  mask_sprite->fade(fade, 1000);
}

void Tile::setHighlighted(bool flag)
{
  if (!lock_highlight)
  {
    highlighted = flag;
  }
}

void Tile::setHealth(const float& new_health)
{
  health = new_health;
  if (health <= 0)
  {
    tile_sprite->setCurrentSprite(1);
  }
}