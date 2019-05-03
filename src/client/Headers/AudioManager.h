#ifndef PROJECT_AUDIOMANAGER_H
#define PROJECT_AUDIOMANAGER_H

#include "gamelib/Headers/Constants.h"
#include <json.hpp>
#include <map>
#include <soloud.h>
#include <soloud_wav.h>
#include <string>

class AudioManager
{
 public:
  AudioManager();
  ~AudioManager();

  void loadSoundFX(const GameLib::SoundFX& clip);
  void loadMusic(const GameLib::MusicTrack& music);
  void unloadAllSoundFiles();

  void playSoundFX(const GameLib::SoundFX& clip,
                   const float& volume = 1,
                   const bool& loop = false);
  void stopSoundFX(const GameLib::SoundFX& clip);

  void playMusic(const GameLib::MusicTrack& music,
                 const float& volume = 1,
                 const bool& loop = false);
  void stopMusic(const GameLib::MusicTrack& music);

  void setMusicVolume(float new_volume);
  void setFXVolume(float new_volume);

  float getMusicVolume() { return music_volume; };
  float getFXVolume() { return fx_volume; };

 private:
  SoLoud::Soloud audio_engine;
  nlohmann::json sound_paths;
  std::map<GameLib::SoundFX, SoLoud::Wav> loaded_fx;
  std::map<GameLib::MusicTrack, SoLoud::Wav> loaded_music;

  // Some variables to keep track of the current music track
  // Used so that we can re-start the music when the volume is adjusted
  GameLib::MusicTrack current_music = GameLib::MusicTrack::NONE;
  bool current_looping = false;
  float current_volume = 0;

  float music_volume = 1;
  float fx_volume = 1;
};

#endif // PROJECT_AUDIOMANAGER_H
