#ifndef PROJECT_LOCATOR_H
#define PROJECT_LOCATOR_H

#include <bits/unique_ptr.h>

class InputManager;
class NetworkManager;
class GameRenderer;
class AudioManager;

class Locator
{
 public:
  Locator() = default;

  static std::unique_ptr<Locator> instance;

  void setNetworkManager(NetworkManager* new_net_manager)
  {
    net_manager = new_net_manager;
  };
  NetworkManager* getNetworkManager() { return net_manager; };

  void setInputManager(InputManager* new_manager)
  {
    inputManager = new_manager;
  };
  InputManager* getInputManager() { return inputManager; };

  void setRenderer(GameRenderer* rend) { renderer = rend; };
  GameRenderer* getRenderer() { return renderer; };

  void setAudioManager(AudioManager* audio) { audio_manager = audio; };
  AudioManager* getAudioManager() { return audio_manager; };

 private:
  NetworkManager* net_manager = nullptr;
  InputManager* inputManager = nullptr;
  GameRenderer* renderer = nullptr;
  AudioManager* audio_manager = nullptr;
};

#endif // PROJECT_LOCATOR_H
