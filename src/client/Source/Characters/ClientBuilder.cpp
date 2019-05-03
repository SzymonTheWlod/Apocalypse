#include "client/Headers/Characters/ClientBuilder.h"
#include "client/Headers/Locator.h"

ClientBuilder::ClientBuilder()
{
  character_type = GameLib::CharacterType::BUILDER;
  sprite = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                            "BuilderPlayer.png");
  highlight = std::make_unique<AnimatedSprite>("data/Character_Icons/"
                                               "BuilderPlayer_highlight.png");

  click.setHeight(sprite->height());
  click.setWidth(sprite->width());
  click.setPosition(position);
}