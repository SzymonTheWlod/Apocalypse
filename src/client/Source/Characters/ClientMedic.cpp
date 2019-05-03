#include "client/Headers/Characters/ClientMedic.h"
#include "client/Headers/Locator.h"

ClientMedic::ClientMedic()
{
  character_type = GameLib::CharacterType::MEDIC;
  sprite = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                            "MedicPlayer.png");
  highlight = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                               "MedicPlayer_highlight.png");
  click.setHeight(sprite->height());
  click.setWidth(sprite->width());
  click.setPosition(position);
}