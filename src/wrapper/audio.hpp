#pragma once
#include "../../libs/SDL2/include/SDL.h"
#include "../../libs/SDL2_mixer/include/SDL_mixer.h"
#include <string>
#include <unordered_map>
#include <iostream>

namespace Audio {

class Manager {
public:
    static bool init(int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 2048);
    static void quit();

    static bool playSound(const std::string& path, int loops = 0); // efek suara
    static bool playMusic(const std::string& path, bool loop = true); // musik background
    static void stopMusic();

private:
    static std::unordered_map<std::string, Mix_Chunk*> loadedSounds;
    static Mix_Music* currentMusic;
};

} 