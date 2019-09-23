#include "song.h"
#include <QMediaPlayer>
#include <iostream>
#include <chrono>
#include <sstream>
#include <QObject>

Song::Song(QString &path, QMediaPlayer* mediaPlayer, QObject *qParent) noexcept {
    pathname = path;
    parent = qParent;
    player = mediaPlayer;
    this->getSongMetaData();
}

Song::~Song() {
//    delete player;
//    player = nullptr;
}

//std::ostream& Song::operator << (std::ostream& os) noexcept {
//    os << title.toStdString() << ", " << album.toStdString() << ", " << artist.toStdString() << ", " << formatDurationToQString().toStdString();
//    return os;
//}

Song::operator const char *() noexcept {
    std::stringstream ss;
    ss << title.toStdString() << ", "  << album.toStdString() << ", " << artist.toStdString() << ", " << formatDurationToQString().toStdString();
//    return ss.str().c_str();
    std::cout << this->getTitle() << std::endl;
    return "test str";
}

std::string Song::getTitle() noexcept {
    std::string replacement = "replacement";
    if (this->title.isEmpty()) return replacement;
    else return this->title.toStdString();
}

//void Song::getSongInfo() noexcept {
//    player = new QMediaPlayer();
//    QObject::connect(player,
//            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
//            this,
//            SLOT(onMediaChanged(QMediaPlayer::MediaStatus)));
//    player->setMedia(QUrl::fromLocalFile(pathname));
//}

void Song::onMediaChanged(QMediaPlayer::MediaStatus status) noexcept {
    if (status == QMediaPlayer::LoadedMedia) {
        std::cout << "here" << std::endl;
//        this->getSongMetaData();

    }
}

void Song::getSongMetaData() noexcept {
    QStringList metadatalist = this->player->availableMetaData();
    int list_size = metadatalist.size();
    QString metadata_key;
    duration = static_cast<int>(player->duration());

    for (int i = 0; i < list_size; ++i)
    {
      metadata_key = metadatalist.at(i);

      if (metadata_key == "Album") {
          album = player->metaData(metadata_key).toString();
      }
      if (metadata_key == "Title") {
          title = player->metaData(metadata_key).toString();
      }
      if (metadata_key == "Artist") {
          artist = player->metaData(metadata_key).toString();
      }
    }
}

QString&& Song::formatDurationToQString() noexcept {
    auto durationInMS = std::chrono::milliseconds(duration);
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(durationInMS);
    durationInMS -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
    secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);

    std::stringstream ss;
    ss << mins.count() << ":" << secs.count();
    return QString(ss.str().c_str());
}

