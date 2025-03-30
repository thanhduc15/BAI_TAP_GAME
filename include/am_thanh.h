#ifndef AM_THANH_H
#define AM_THANH_H

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>

class AmThanh {
public:
    static AmThanh& getInstance();

    bool init();
    void loadSound(const std::string& id, const std::string& filePath);
    void loadMusic(const std::string& filePath);

    void playSound(const std::string& id, int loops = 0);
    void playMusic(int loops = -1);
    void stopMusic();

    void setMusicVolume(int volume);
    void setSoundVolume(const std::string& id, int volume);

    void clean();

private:
    AmThanh() {}
    ~AmThanh() { clean(); }

    std::map<std::string, Mix_Chunk*> soundEffects;
    Mix_Music* music = nullptr;
};

#endif
