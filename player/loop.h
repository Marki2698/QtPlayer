#ifndef LOOP_H
#define LOOP_H

#include "types.h"


class Loop {
public:
    Loop() {}
    ~Loop() {}
    void nextLoopMode() noexcept;
    playModeAndPixMapT getCurrentLoopMode() noexcept;
private:
    std::vector<playModeAndPixMapT> modes {
        { QMediaPlaylist::PlaybackMode::Sequential, QPixmap(":/recources/images/loop_off.png") },
        { QMediaPlaylist::PlaybackMode::Loop, QPixmap(":/recources/images/loop.png") },
        { QMediaPlaylist::PlaybackMode::CurrentItemInLoop, QPixmap(":/recources/images/loop_one.png") }
    };
    size_t modeIndex = 1;
    playModeAndPixMapT currentMode = modes[0];
};

#endif // LOOP_H
