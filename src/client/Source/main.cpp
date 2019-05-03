#include "client/Headers/game.h"
#include <gamelib/Headers/gamelib.h>
int main()
{
  MyASGEGame asge_game;
  asge_game.init();
  asge_game.run();
  return 0;
}