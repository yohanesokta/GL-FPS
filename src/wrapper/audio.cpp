#include "audio.hpp"

namespace Audio {

std::unordered_map<std::string, Mix_Chunk*> Manager::loadedSounds;
Mix_Music* Manager::currentMusic = nullptr;

bool Manager::init(int frequency, Uint16 format, int channels, int chunksize) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! " << SDL_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
        std::cerr << "SDL_mixer could not initialize! " << Mix_GetError() << "\n";
        return false;
    }

    Mix_AllocateChannels(16); // maksimal 16 channel efek
    return true;
}

void Manager::quit() {
    for (auto& pair : loadedSounds) {
        Mix_FreeChunk(pair.second);
    }
    loadedSounds.clear();

    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    Mix_CloseAudio();
    SDL_Quit();
}

bool Manager::playSound(const std::string& path, int loops) {
    Mix_Chunk* chunk = nullptr;

    // cek apakah sudah di-load
    auto it = loadedSounds.find(path);
    if (it != loadedSounds.end()) {
        chunk = it->second;
    } else {
        chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "Failed to load sound: " << path << " | " << Mix_GetError() << "\n";
            return false;
        }
        loadedSounds[path] = chunk;
    }

    if (Mix_PlayChannel(-1, chunk, loops) == -1) {
        std::cerr << "Failed to play sound: " << path << " | " << Mix_GetError() << "\n";
        return false;
    }

    return true;
}

bool Manager::playMusic(const std::string& path, bool loop) {
    if (currentMusic) {
        Mix_HaltMusic();
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    currentMusic = Mix_LoadMUS(path.c_str());
    if (!currentMusic) {
        std::cerr << "Failed to load music: " << path << " | " << Mix_GetError() << "\n";
        return false;
    }

    if (Mix_PlayMusic(currentMusic, loop ? -1 : 1) == -1) {
        std::cerr << "Failed to play music: " << path << " | " << Mix_GetError() << "\n";
        return false;
    }

    return true;
}

void Manager::stopMusic() {
    Mix_HaltMusic();
}

} // namespace Audio