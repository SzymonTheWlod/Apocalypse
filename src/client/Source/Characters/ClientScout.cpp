#include "client/Headers/Characters/ClientScout.h"
#include "client/Headers/Locator.h"

ClientScout::ClientScout()
{
  character_type = GameLib::CharacterType::SCOUT;
  sprite = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                            "ScoutPlayer.png");
  highlight = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                               "ScoutPlayer_highlight.png");
  click.setHeight(sprite->height());
  click.setWidth(sprite->width());
  click.setPosition(position);
}
