#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <QMediaPlaylist>
#include <QPixmap>
#include <vector>

class Shuffle {
public:
    Shuffle() {}
    ~Shuffle() {}
    std::pair<QMediaPlaylist::PlaybackMode, QPixmap> nextShuffleMode() noexcept;
private:
    std::vector<std::pair<QMediaPlaylist::PlaybackMode, QPixmap>> modes {
        { QMediaPlaylist::PlaybackMode::Sequential, QPixmap(":/recources/images/shuffle_off.png") },
        { QMediaPlaylist::PlaybackMode::Random, QPixmap(":/recources/images/shuffle_on.png") }
    };
    size_t modeIndex = 1;
};
#endif // SHUFFLE_H
