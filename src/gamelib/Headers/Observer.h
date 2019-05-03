#ifndef PLAY_AND_GAMES_REDUX_APOCALYPSE_OBSERVER_H
#define PLAY_AND_GAMES_REDUX_APOCALYPSE_OBSERVER_H

#include "Constants.h"
#include "NetworkPacket.h"

namespace GameLib
{
class Observer
{
 public:
  virtual ~Observer() = default;
  virtual void onNotify(NetworkPacket& data) = 0;
};
}

#endif // PLAY_AND_GAMES_REDUX_APOCALYPSE_OBSERVER_H
