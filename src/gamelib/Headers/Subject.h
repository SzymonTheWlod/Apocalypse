#ifndef PROJECT_SUBJECT_H
#define PROJECT_SUBJECT_H

#include "gamelib/Headers/Observer.h"
#include <map>
#include <vector>

namespace GameLib
{
class Subject
{
 public:
  Subject() = default;
  ~Subject() = default;

  void addObserver(Observer* observer, GameLib::EventType observed_event);
  void removeObserver(Observer* observer, GameLib::EventType observed_event);

 protected:
  void notify(GameLib::EventType& type, NetworkPacket& data);

 private:
  std::multimap<GameLib::EventType, Observer*> observers;
};
}

#endif // PROJECT_SUBJECT_H
