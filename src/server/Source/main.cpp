#include "gamelib/Headers/Printer.h"
#include "server/Headers/Locator.h"
#include "server/Headers/Map.h"
#include "server/Headers/Network/NetworkManager.h"
#include "server/Headers/Scenes/Lobby.h"
#include "server/Headers/Scenes/MainGame.h"
#include "server/Headers/Scenes/ServerCharacterSelect.h"
#include "server/Headers/ServerTile.h"

int main()
{
  std::unique_ptr<NetworkManager> network_manager =
    std::make_unique<NetworkManager>();
  Locator::instance = std::make_unique<Locator>();

  Locator::instance->setNetworkManager(network_manager.get());

  std::unique_ptr<ServerScene> scene = std::make_unique<Lobby>();
  scene->load();

  while (network_manager->checkServerActive())
  {
    switch (scene->update())
    {
      case GameLib::SceneType::CHAR_SELECT_SCENE:
      {
        scene =
          std::make_unique<ServerCharacterSelect>(scene->getPersistentData());
        scene->load();
        break;
      }
      case GameLib::SceneType::GAME_SCENE:
      {
        scene = std::make_unique<MainGame>(scene->getPersistentData());
        scene->load();
        break;
      }
      case GameLib::SceneType::MENU_SCENE:
      {
        scene = std::make_unique<Lobby>();
        scene->load();
        break;
      }
      default:
        break;
    }
    network_manager->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 0;
}