#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <QMediaPlaylist>
#include <QPixmap>
#include <vector>

#include "types.h"

class Shuffle {
public:
    Shuffle() {}
    ~Shuffle() {}
    playModeAndPixMapT nextShuffleMode() noexcept;
private:
    std::vector<playModeAndPixMapT> modes {
        { QMediaPlaylist::PlaybackMode::Sequential, QPixmap(":/recources/images/shuffle_off.png") },
        { QMediaPlaylist::PlaybackMode::Random, QPixmap(":/recources/images/shuffle_on.png") }
    };
    size_t modeIndex = 1;
};
#endif // SHUFFLE_H
