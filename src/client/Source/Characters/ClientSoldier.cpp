#include "client/Headers/Characters/ClientSoldier.h"
#include "client/Headers/Locator.h"

ClientSolider::ClientSolider()
{
  character_type = GameLib::CharacterType::SOLDIER;
  sprite = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                            "SoldierPlayer.png");
  highlight = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                               "SoldierPlayer_highlight.png");
  click.setHeight(sprite->height());
  click.setWidth(sprite->width());
  click.setPosition(position);
}