#include "client/Headers/UI/UI.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/Locator.h"

void UI::moveBy(Point point)
{
  position = position + point;
}