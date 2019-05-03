#include "client/Headers/GameRenderer.h"
#include "client/Headers/AnimatedSprite.h"
#include "client/Headers/FileReader.h"
#include <json.hpp>
using json = nlohmann::json;

GameRenderer::GameRenderer(ASGE::Renderer* rend) : renderer(rend)
{
  // Read sound levels from config
  json config;
  // Check to see if there is already saved data
  if (!FileReader().readJsonFile("SavedConfigs.json", config))
  {
    // If not read the saved data file using the original values
    FileReader().readJsonFile("data/config/GameSetup.json", config);
  }
  renderer->setWindowedMode(
    static_cast<ASGE::Renderer::WindowMode>(config["window_mode"]));
}

void GameRenderer::renderSpriteToWorld(ASGE::Sprite* sprite)
{
  // Add the camera offset to the sprite
  sprite->xPos((sprite->xPos() * zoom) - position.x);
  sprite->yPos((sprite->yPos() * zoom) - position.y);
  sprite->scale(sprite->scale() * zoom);
  // Render ths sprite
  renderer->renderSprite(*sprite);

  // Restore the sprite to the previous position
  sprite->xPos((sprite->xPos() + position.x) / zoom);
  sprite->yPos((sprite->yPos() + position.y) / zoom);
  sprite->scale(sprite->scale() / zoom);
}

void GameRenderer::renderSpriteToScreen(ASGE::Sprite* sprite)
{
  renderer->renderSprite(*sprite);
}

void GameRenderer::renderTextToWorld(std::string& text,
                                     Point pos,
                                     float& scale,
                                     ASGE::Colour& colour)
{
  pos - position;
  renderer->renderText(
    text, static_cast<int>(pos.x), static_cast<int>(pos.y), scale, colour);
}

void GameRenderer::renderTextToScreen(std::string& text,
                                      Point pos,
                                      float& scale,
                                      ASGE::Colour& colour)
{
  renderer->renderText(
    text, static_cast<int>(pos.x), static_cast<int>(pos.y), scale, colour);
}

Point GameRenderer::displayedToSimulatedWorld(Point point)
{
  point = point + position;
  point = point / zoom;
  point = point / AnimatedSprite::width_scale;
  return point;
}

void GameRenderer::resetCamera()
{
  zoom = 1;
  position = Point(0, 0);
}

void GameRenderer::moveCamera(Point amount)
{
  position = position + amount;

  if (position.x < lower_limit.x)
  {
    position.x = lower_limit.x;
  }
  else if (position.x > upper_limit.x * zoom)
  {
    position.x = upper_limit.x * zoom;
  }
  if (position.y < lower_limit.y)
  {
    position.y = lower_limit.y;
  }
  else if (position.y > upper_limit.y * zoom)
  {
    position.y = upper_limit.y * zoom;
  }
}

void GameRenderer::zoomCamera(float zoom_amount, Point zoom_focus)
{
  zoom += zoom_amount;
  if (zoom > max_zoom)
  {
    zoom_amount -= zoom - max_zoom;
    zoom = max_zoom;
  }
  else if (zoom < min_zoom)
  {
    zoom_amount -= zoom - min_zoom;
    zoom = min_zoom;
  }
  position = position + (zoom_focus * zoom_amount);
}

void GameRenderer::cycleWindowMode()
{
  if (renderer->getWindowMode() == ASGE::Renderer::WindowMode::WINDOWED)
  {
    renderer->setWindowedMode(ASGE::Renderer::WindowMode ::BORDERLESS);
  }
  else if (renderer->getWindowMode() == ASGE::Renderer::WindowMode::BORDERLESS)
  {
    renderer->setWindowedMode(ASGE::Renderer::WindowMode ::WINDOWED);
  }
  // Read sound levels from config
  json config;
  // Check to see if there is already saved data
  if (!FileReader().readJsonFile("SavedConfigs.json", config))
  {
    // If not read the saved data file using the original values
    FileReader().readJsonFile("data/config/GameSetup.json", config);
  }
  config["window_mode"] = static_cast<int>(renderer->getWindowMode());
  FileReader().writeToJsonFile("SavedConfigs.json", config);
}

void GameRenderer::setCameraLimits(const Point& lower, const Point& upper)
{
  lower_limit = lower;
  upper_limit = upper;
}