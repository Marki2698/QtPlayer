#include "loop.h"
//#include <QMediaPlaylist>
//#include <QPixmap>

void Loop::nextLoopMode() noexcept {
    if (modeIndex == modes.size()) modeIndex = 0;
    currentMode = modes[modeIndex++];
}

playModeAndPixMapT Loop::getCurrentLoopMode() noexcept {
    return currentMode;
}

