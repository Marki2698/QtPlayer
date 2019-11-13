#ifndef SONG_H
#define SONG_H

#include "types.h"


class Song
{
public:
    explicit Song(QString& path, QString&& tit, QString&& alb, QString&& art, int&& dur) noexcept;
    Song(Song&& source) noexcept;
    Song& operator =(Song&& source) noexcept;
    ~Song();
    operator const char* () noexcept;
//    friend std::ostream& operator << (std::ostream& os, const Song& song) noexcept;
    std::string getTitle() noexcept;
    std::string getStdPath() noexcept;
    QString getQStrPath() noexcept;
    QString show() const noexcept;
    static std::unordered_map<std::string, std::shared_ptr<Song>> getSongsMap(const std::vector<std::string>& songsPathes) noexcept;

    private:
        QString pathname;
        QString title;
        QString album;
        QString artist;
        unsigned int duration;
        QString formatDurationToQString() const noexcept;
};

#endif // SONG_H
