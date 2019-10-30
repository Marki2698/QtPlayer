#include "shuffle.h"
//#include <QMediaPlaylist>
//#include <QPixmap>

playModeAndPixMapT Shuffle::nextShuffleMode() noexcept {
    if (modeIndex == modes.size()) modeIndex = 0;
    return modes[modeIndex++];
}

