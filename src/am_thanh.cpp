#include "am_thanh.h"
#include <iostream>

AmThanh& AmThanh::getInstance() {
    static AmThanh instance;
    return instance;
}

bool AmThanh::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
       // std::cerr << "Lỗi khởi tạo SDL_mixer! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void AmThanh::loadSound(const std::string& id, const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (sound == nullptr) {
       // std::cerr << "Lỗi tải hiệu ứng âm thanh! Mix_Error: " << Mix_GetError() << std::endl;
        return;
    }
    soundEffects[id] = sound;
}

void AmThanh::loadMusic(const std::string& filePath) {
    music = Mix_LoadMUS(filePath.c_str());
    if (music == nullptr) {
        //std::cerr << "Lỗi tải nhạc nền! Mix_Error: " << Mix_GetError() << std::endl;
    }
}

void AmThanh::playSound(const std::string& id, int loops) {
    if (soundEffects.find(id) != soundEffects.end()) {
        Mix_PlayChannel(-1, soundEffects[id], loops);
    }
}

void AmThanh::playMusic(int loops) {
    if (music != nullptr) {
        Mix_PlayMusic(music, loops);
    }
}

void AmThanh::stopMusic() {
    Mix_HaltMusic();
}

void AmThanh::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void AmThanh::setSoundVolume(const std::string& id, int volume) {
    if (soundEffects.find(id) != soundEffects.end()) {
        Mix_VolumeChunk(soundEffects[id], volume);
    }
}

void AmThanh::clean() {
    for (auto& sound : soundEffects) {
        Mix_FreeChunk(sound.second);
    }
    soundEffects.clear();

    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();
}
