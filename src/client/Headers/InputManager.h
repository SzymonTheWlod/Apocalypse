#ifndef PROJECT_KEYINPUTMANAGER_H
#define PROJECT_KEYINPUTMANAGER_H

#include "gamelib/Headers/Point.h"
#include "gamelib/Headers/Subject.h"
#include <Engine/Input.h>
#include <Engine/InputEvents.h>

class InputManager : public GameLib::Subject
{
 public:
  void processKeyData(const ASGE::KeyEvent* data);
  void processClickData(const ASGE::ClickEvent* data);
  void processScrollData(const ASGE::ScrollEvent* data);

  void setInput(ASGE::Input* input_object) { input = input_object; };
  Point getWorldMousePosition();
  Point getScreenMousePosition();

 private:
  ASGE::Input* input = nullptr;
};

#endif // PROJECT_KEYINPUTMANAGER_H
