#ifndef PROJECT_PERSISTANTSCENEDATA_H
#define PROJECT_PERSISTANTSCENEDATA_H

#include "gamelib/Headers/Constants.h"
#include <string>

struct PersistentClientData
{
  GameLib::CharacterType selected_char = GameLib::CharacterType ::NONE;
  std::string username;
};

#endif // PROJECT_PERSISTANTSCENEDATA_H
