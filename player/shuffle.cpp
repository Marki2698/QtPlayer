#include "shuffle.h"
#include <QMediaPlaylist>
#include <QPixmap>

std::pair<QMediaPlaylist::PlaybackMode, QPixmap> Shuffle::nextShuffleMode() noexcept {
    if (modeIndex == modes.size()) modeIndex = 0;
    return modes[modeIndex++];
}

