#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H

namespace GameLib
{
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
const float ASPECT_RATIO = 16.0f / 9.0f;

enum class EventType : int
{
  DEFAULT,
  // Simple client/server events that ensure the server waits for clients before
  // sending certain bits of data All of these have no data other than their
  // type
  CONNECT_TO_SERVER,
  CLIENT_CONNECTED,
  CLIENT_DISCONNECTED,
  CLIENT_RECONNECTED,
  CLIENT_READY_TO_START,
  NUMBER_OF_CLIENTS_READY, // << int << int
  SET_USERNAME,            // << std::string
  SET_ID,                  // << unsigned int
  SET_CHARACTER,           // << enum
  CLAIM_CHARACTER,         // << enum

  // Input events
  KEY_PRESSED_ANY, // << ASGE::KeyEvent
  MOUSE_CLICKED,   // << ASGE::ClickEvent
  MOUSE_SCROLLED,  // << ASGE::ScrollEvent

  // InputText event
  INPUT_SUBMITTED, // << std::string
  CHAT_MESSAGE,    // << std::string
  SHOW_CHAT,
  HIDE_CHAT,
  OPEN_INSTRUCTIONS,

  SERVER_CHANGE_SCENE, // << SceneType

  // Char select events
  MEDIC_CHOSEN,
  SCOUT_CHOSEN,
  BUILDER_CHOSEN,
  SOLDIER_CHOSEN,

  CREATE_CHARACTER,  // enum
  MOVE_CHARACTER,    // enum << size_t
  SET_ACTION_POINTS, // << int
  START_TURN,
  END_TURN,

  EVENTS,
  DISASTER_BAR, // Double
  TORNADO,      // TEST EVENT

  MAP_DATA, // unsigned long long << unsigned long long << unsigned long long <<
            // TileType //first unsigned Long Long is number of tiles sent first
            // int
  REVEAL_TILE,           // << size_t << float
  SCOUT_TILE,            // << enum
  SET_SCOUTED,           // << size_t
  SCAVENGE_TILE,         // << enum
  GIVE_PLAYER_RESOURCES, // << enum << Resources
  SET_PLAYER_HEALTH,     // enum << float
  SET_TILE_RESOURCES,    // size_t << Resources
  SET_TILE_HEALTH,       // << size_t << float
  BUILD_BUNKER,
  DEPOSIT_RESOURCES,
  INCREMENT_A_RESOURCES,
  HEAL_PLAYER,
  SET_WIN_CONDITION,
  WIN_GAME,
  LOSE_GAME,

  // PopupUi
  CLOSE_TILEINFO,
  CLOSE_PLAYERINFO,

  // Options
  MUSIC_VOLUME_UP,
  MUSIC_VOLUME_DOWN,
  FX_VOLUME_UP,
  FX_VOLUME_DOWN,
  CLOSE_WINDOW,
  OPEN_WINDOW,
  WINDOW_MODE,
  BACK_TO_MENU,
  EXIT_GAME,
  SIGNAL_EXIT

};

enum class SceneType : int
{
  SIGNAL_EXIT = -2,
  NO_CHANGE = -4,
  TITLE_SCENE = -1,
  MENU_SCENE = 1,
  CHAR_SELECT_SCENE = 2,
  GAME_SCENE = 3

};

// Rotation used for progress bars
enum Rotation
{
  LEFT_TO_RIGHT = 0,
  UP_TO_DOWN = 1,
};

enum class TileType : int
{
  NONE = -1,
  TOWN_HALL = 0,
  HOUSES = 1,
  FLATS = 2,
  HOSPITAL = 3,
  SCHOOL = 4,
  POLICE_STATION = 5,
  FIRE_STATION = 6,
  UNIVERSITY = 7,
  WATER_PLANT = 8,
  GRASS = 9,
  SUPERMARKET = 10

};

enum class CharacterType : int
{
  MEDIC = 0,
  SCOUT = 1,
  SOLDIER = 2,
  BUILDER = 3,
  NONE = 4
};

enum class SoundFX : int
{
  NONE = -1,
  AVALANCHE = 0,
  EXPLOSION = 1,
  THUNDER = 2,
  TORNADO = 3,
  TREMER = 4,
  TSUNAMI = 5,
  WILDFIRE = 6,
  HEAL = 7,
  MOVE = 8,
  SCAVANGE = 9,
  SCOUT = 10,
  YOUR_TURN = 11,
  METEOR = 12

};

enum class MusicTrack : int
{
  NONE = -1,
  MENU_TRACK = 0,
  BACKGROUND_TRACK = 1
};

enum class DisasterType : int
{
  TOP_ROW = 0,
  BOTTOM_ROW = 1,
  RAND_COLUMN = 2,
  RAND_TILE = 3

};

}
#endif // PROJECT_CONSTANTS_H
