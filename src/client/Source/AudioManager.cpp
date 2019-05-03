#include "client/Headers/AudioManager.h"
#include "client/Headers/FileReader.h"
#include "gamelib/Headers/Printer.h"
#include <Engine/FileIO.h>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

AudioManager::AudioManager()
{
  audio_engine.init();
  FileReader().readJsonFile("data/config/AudioClips.json", sound_paths);

  // Read sound levels from config
  json config;
  // Check to see if there is already saved data
  if (!FileReader().readJsonFile("SavedConfigs.json", config))
  {
    // If not read the saved data file using the original values
    FileReader().readJsonFile("data/config/GameSetup.json", config);
  }

  music_volume = config["music_volume"];
  fx_volume = config["fx_volume"];
}

AudioManager::~AudioManager()
{
  audio_engine.deinit();
}

void AudioManager::loadSoundFX(const GameLib::SoundFX& clip)
{
  if (loaded_fx.count(clip))
  {
    return;
  }
  loaded_fx[clip] = SoLoud::Wav();

  auto file = ASGE::FILEIO::File();
  file.open(sound_paths["fx"][std::to_string(static_cast<int>(clip))]);
  auto buffer = file.read();

  loaded_fx[clip].loadMem(buffer.as_unsigned_char(),
                          static_cast<unsigned int>(buffer.length),
                          false,
                          false);
}

void AudioManager::loadMusic(const GameLib::MusicTrack& music)
{
  if (loaded_music.count(music))
  {
    return;
  }
  loaded_music[music] = SoLoud::Wav();

  auto file = ASGE::FILEIO::File();
  file.open(sound_paths["music"][std::to_string(static_cast<int>(music))]);
  auto buffer = file.read();

  loaded_music[music].loadMem(buffer.as_unsigned_char(),
                              static_cast<unsigned int>(buffer.length),
                              false,
                              false);
}

void AudioManager::unloadAllSoundFiles()
{
  loaded_fx.clear();
}

void AudioManager::playSoundFX(const GameLib::SoundFX& clip,
                               const float& volume,
                               const bool& loop)
{
  if (loaded_fx.count(clip) && fx_enabled)
  {
    loaded_fx[clip].setLooping(loop);
    audio_engine.play(loaded_fx[clip], volume * fx_volume);
  }
}

void AudioManager::stopSoundFX(const GameLib::SoundFX& clip)
{
  if (loaded_fx.count(clip))
  {
    audio_engine.stopAudioSource(loaded_fx[clip]);
  }
}

void AudioManager::playMusic(const GameLib::MusicTrack& music,
                             const float& volume,
                             const bool& loop)
{
  if (loaded_music.count(music))
  {
    current_looping = loop;
    current_volume = volume;
    if (current_music != GameLib::MusicTrack::NONE)
    {
      stopMusic(current_music);
    }
    current_music = music;
    loaded_music[music].setLooping(loop);
    audio_engine.play(loaded_music[music], volume * music_volume);
  }
}

void AudioManager::stopMusic(const GameLib::MusicTrack& music)
{
  if (loaded_music.count(music))
  {
    audio_engine.stopAudioSource(loaded_music[music]);
  }
}

void AudioManager::setFXVolume(float new_volume)
{
  fx_volume = new_volume;
  fx_volume = new_volume;
  if (fx_volume > 1)
  {
    fx_volume = 1;
  }
  else if (fx_volume < 0)
  {
    fx_volume = 0;
  }
  json config;
  // Check to see if there is already saved data
  if (!FileReader().readJsonFile("SavedConfigs.json", config))
  {
    // If not create the saved data file using the original values
    FileReader().readJsonFile("data/config/GameSetup.json", config);
  }

  config["fx_volume"] = fx_volume;
  FileReader().writeToJsonFile("SavedConfigs.json", config);
}

void AudioManager::setMusicVolume(float new_volume)
{
  music_volume = new_volume;
  if (music_volume > 1)
  {
    music_volume = 1;
  }
  else if (music_volume < 0)
  {
    music_volume = 0;
  }
  json config;
  // Check to see if there is already saved data
  if (!FileReader().readJsonFile("SavedConfigs.json", config))
  {
    // If not create the saved data file using the original values
    FileReader().readJsonFile("data/config/GameSetup.json", config);
  }

  config["music_volume"] = music_volume;
  FileReader().writeToJsonFile("SavedConfigs.json", config);

  // Restart the music
  playMusic(current_music, current_volume, current_looping);
}