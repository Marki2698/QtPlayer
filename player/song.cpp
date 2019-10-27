#include "song.h"
#include <QMediaPlayer>
#include <iostream>
#include <chrono>
#include <sstream>
#include <string>
#include <memory>
#include <unordered_map>
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/audioproperties.h"


Song::Song(QString &path, QString&& tit, QString&& alb, QString&& art, int&& dur) noexcept {
    pathname = path;
    album = std::move(alb);
    artist = std::move(art);
    title = std::move(tit);
    std::chrono::seconds sec { std::move(dur) };
    std::chrono::milliseconds ms { std::chrono::duration_cast<std::chrono::milliseconds>(sec) };
    duration = ms.count();
}

Song::~Song() {}

//std::ostream& operator << (std::ostream& os, const Song& song) noexcept {
//    os << song.pathname.toStdString().c_str() << ", " << song.title.toStdString().c_str() << ", " << song.album.toStdString().c_str() << ", " << song.artist.toStdString().c_str() << ", " << song.duration << std::endl;
//    return os;
//}

Song::operator const char* () noexcept {
    std::stringstream ss;
    ss << pathname.toStdString() << "\t" << title.toStdString() << "\t"  << artist.toStdString() << "\t" << album.toStdString() << "\t" << duration;
    std::cout << ss.str() << std::endl;
    std::wstring out(ss.str().cbegin(), ss.str().cend());
//    return ss.str().c_str();
//    std::wstring str = L"test";
    std::string str = "markiian lukashiv";
    return str.c_str();
}

std::string Song::getTitle() noexcept {
    return this->title.toStdString();
}

std::string Song::getStdPath() noexcept {
    return this->pathname.toStdString();
}

QString Song::getQStrPath() noexcept {
    return this->pathname;
}

QString Song::show() const noexcept {
    return this->title + this->album + this->artist + this->formatDurationToQString();
}

std::unordered_map<std::string, std::shared_ptr<Song>> Song::getSongsMap(const std::vector<std::string>& songsPathes) noexcept {
    std::unordered_map<std::string, std::shared_ptr<Song>> songsMap;
    for (const std::string& path : songsPathes) {
        TagLib::FileRef f(path.c_str());
        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();

            QString Qpath = path.c_str();
            std::shared_ptr<Song> songPtr = std::make_shared<Song>(Qpath,
                                                                   std::move(QString(tag->title().to8Bit(true).c_str())),
                                                                   std::move(QString(tag->album().to8Bit(true).c_str())),
                                                                   std::move(QString(tag->artist().to8Bit(true).c_str())),
                                                                   std::move(f.audioProperties()->length())
                                                                   );
            songsMap.insert(std::make_pair(songPtr->show().toStdString(), std::move(songPtr)));
        }
    }

    return songsMap;
}

QString Song::formatDurationToQString() const noexcept {
    auto durationInMS = std::chrono::milliseconds(duration);
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(durationInMS);
    durationInMS -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
    secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);

    std::stringstream ss;
    ss << mins.count() << ":" << secs.count();
    return std::move(QString(ss.str().c_str()));
}

