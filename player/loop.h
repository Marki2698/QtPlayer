#ifndef LOOP_H
#define LOOP_H

#include <QMediaPlaylist>
#include <QPixmap>
#include <vector>

class Loop {
public:
    Loop() {}
    ~Loop() {}
    std::pair<QMediaPlaylist::PlaybackMode, QPixmap> nextLoopMode() noexcept;
private:
    std::vector<std::pair<QMediaPlaylist::PlaybackMode, QPixmap>> modes {
        { QMediaPlaylist::PlaybackMode::Sequential, QPixmap(":/recources/images/loop_off.png") },
        { QMediaPlaylist::PlaybackMode::Loop, QPixmap(":/recources/images/loop.png") },
        { QMediaPlaylist::PlaybackMode::CurrentItemInLoop, QPixmap(":/recources/images/loop_one.png") }
    };
    size_t modeIndex = 1;
};

#endif // LOOP_H
