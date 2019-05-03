#ifndef PROJECT_CHARACTERMANAGER_H
#define PROJECT_CHARACTERMANAGER_H

#include "client/Headers/Characters/Character.h"
#include "client/Headers/Characters/ClientBuilder.h"
#include "client/Headers/Characters/ClientMedic.h"
#include "client/Headers/Characters/ClientScout.h"
#include "client/Headers/Characters/ClientSoldier.h"
#include "client/Headers/Map.h"
#include "client/Headers/Tile.h"
#include "gamelib/Headers/Observer.h"
#include <map>

class CharacterManager : public GameLib::Observer
{
 public:
  CharacterManager();
  ~CharacterManager() override;

  void createCharacter(GameLib::CharacterType character);
  void moveCharacter(GameLib::CharacterType character, Tile* tile);
  void render(double delta_time);
  void update(double delta_time);

  void onNotify(GameLib::NetworkPacket& data) override;
  void setLocalCharacter(GameLib::CharacterType type);
  Character* getLocalCharacter() { return characters[local_character].get(); };
  Character* checkPlayerClicked(Point clicked_pos);

  void setMap(Map* new_map) { map = new_map; };

 private:
  GameLib::CharacterType local_character = GameLib::CharacterType ::NONE;
  std::map<GameLib::CharacterType, std::unique_ptr<Character>> characters;

  std::unique_ptr<AnimatedSprite> your_turn = nullptr;
  bool show_text = false;
  double time_elapsed = 0;

  Map* map = nullptr;

  int position_priority = 0;
};

#endif // PROJECT_CHARACTERMANAGER_H
