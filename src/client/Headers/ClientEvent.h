//
// Created by j2-lynham on 05/04/2019.
//

#ifndef PROJECT_CLIENTEVENT_H
#define PROJECT_CLIENTEVENT_H
#include "client/Headers/GameRenderer.h"
#include "client/Headers/UI/ProgressBar.h"
#include "gamelib/Headers/Observer.h"
#include <client/Headers/Map.h>

class ClientEvent : public GameLib::Observer
{
 public:
  ClientEvent();
  ~ClientEvent();
  void onNotify(GameLib::NetworkPacket& data) override;
  void updateEvents();
  void render(double delta_time);

  double barIncrement;
  std::unique_ptr<ProgressBar> sprite = nullptr;
  std::unique_ptr<AnimatedSprite> danger_meter = nullptr;

 private:
  std::unique_ptr<AnimatedSprite> event_sprite = nullptr;
  Map map;
};

#endif // PROJECT_CLIENTEVENT_H
