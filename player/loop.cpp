#include "loop.h"
//#include <QMediaPlaylist>
//#include <QPixmap>

playModeAndPixMapT Loop::nextLoopMode() noexcept {
    if (modeIndex == modes.size()) modeIndex = 0;
    return modes[modeIndex++];
}

