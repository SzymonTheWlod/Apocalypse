#include "gamelib/Headers/Subject.h"
#include <gamelib/Headers/Printer.h>

void GameLib::Subject::addObserver(Observer* observer,
                                   GameLib::EventType observed_event)
{
  using observer_pair = std::pair<GameLib::EventType, Observer*>;
  observers.insert(observer_pair(observed_event, observer));
}

void GameLib::Subject::removeObserver(Observer* observer,
                                      GameLib::EventType observed_event)
{
  auto del_it = observers.equal_range(observed_event).first;
  bool found = false;
  for (auto it = observers.equal_range(observed_event).first;
       it != observers.end();
       it++)
  {
    if (it->second == observer && it->first == observed_event)
    {
      del_it = it;
      found = true;
    }
  }
  if (found)
  {
    observers.erase(del_it);
  }
}

void GameLib::Subject::notify(GameLib::EventType& type,
                              GameLib::NetworkPacket& data)
{
  for (auto it = observers.equal_range(type).first; it != observers.end(); ++it)
  {
    if (it->first == type)
    {
      it->second->onNotify(data);
      data.resetReadPos();
    }
  }
}