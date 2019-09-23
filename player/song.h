#ifndef SONG_H
#define SONG_H

#include <QMediaPlayer>
#include <QObject>
#include <QString>
#include <iostream>
#include <chrono>
#include <string>

class Song: public QObject
{
    Q_OBJECT
public:
    explicit Song(QString& path, QMediaPlayer* mediaPlayer, QObject* qParent = 0) noexcept;
    Song(Song&& source) noexcept;
    Song& operator =(Song&& source) noexcept;
    ~Song();
//    std::ostream& operator << (std::ostream& os) noexcept;
    operator const char* () noexcept;
    std::string getTitle() noexcept;

public slots:
    void onMediaChanged(QMediaPlayer::MediaStatus status) noexcept;

    private:
        QObject* parent;
        QString pathname;
        QString title;
        QString album;
        QString artist;
        unsigned int duration;
        QMediaPlayer* player = nullptr;
//        void getSongInfo() noexcept;
        void getSongMetaData() noexcept;
        QString&& formatDurationToQString() noexcept;
};

#endif // SONG_H
