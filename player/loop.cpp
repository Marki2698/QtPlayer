#include "loop.h"
#include <QMediaPlaylist>
#include <QPixmap>

std::pair<QMediaPlaylist::PlaybackMode, QPixmap> Loop::nextLoopMode() noexcept {
    if (modeIndex == modes.size()) modeIndex = 0;
    return modes[modeIndex++];
}

