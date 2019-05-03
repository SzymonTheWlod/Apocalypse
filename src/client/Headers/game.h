#pragma once
#include "client/Headers/AudioManager.h"
#include "client/Headers/GameRenderer.h"
#include "client/Headers/InputManager.h"
#include "client/Headers/Network/ClientConnection.h"
#include "client/Headers/Network/NetworkManager.h"
#include "client/Headers/Scenes/SceneManager.h"
#include <Engine/OGLGame.h>
#include <string>

/**
 *  An OpenGL Game based on ASGE.
 */
class MyASGEGame : public ASGE::OGLGame
{
 public:
  MyASGEGame();
  ~MyASGEGame();
  virtual bool init() override;

 private:
  void keyHandler(const ASGE::SharedEventData data);
  void clickHandler(const ASGE::SharedEventData data);
  void scrollHandler(const ASGE::SharedEventData data);
  void setupResolution();

  virtual void update(const ASGE::GameTime&) override;
  virtual void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;    /**< Key Input Callback ID. */
  int mouse_callback_id = -1;  /**< Mouse Input Callback ID. */
  int scroll_callback_id = -1; /**< Scroll Input Callback ID. */

  std::unique_ptr<InputManager> input_manager = nullptr;
  std::unique_ptr<SceneManager> scene_manager = nullptr;
  std::unique_ptr<NetworkManager> network_manager = nullptr;
  std::unique_ptr<GameRenderer> game_renderer = nullptr;
  std::unique_ptr<AudioManager> audio_manager = nullptr;
};