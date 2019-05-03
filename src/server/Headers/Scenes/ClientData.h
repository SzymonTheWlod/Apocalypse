#ifndef PROJECT_CLIENTDATA_H
#define PROJECT_CLIENTDATA_H

#include "gamelib/Headers/Constants.h"

struct ClientData
{
  GameLib::CharacterType type = GameLib::CharacterType::NONE;
  bool ready = false;
  bool connected = true;
};

#endif // PROJECT_CLIENTDATA_H
