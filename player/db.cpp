#include "db.h"
#include "fs.h"
#include "statuses.h"
#include "storage.h"
#include "filestorage.h"
#include "utils.h"
//#include "types.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <QStringList>


// add monitor

DB::DB() noexcept {
    //default contructor for creating connection with real DB;
    // storage = make_unique<DBStorage>();
}

DB::DB(const std::string& filename, const std::string& playlistDir) noexcept {
    std::cout << "open connection" << std::endl;
    storage = make_unique<FileStorage>(filename, playlistDir);
}

DB::~DB() noexcept {
    std::cout << "close connection" << std::endl;
}

void DB::addSongs(const songsVectorT &songs) const noexcept {
    storage->addSongs(songs);
}

void DB::addSongToPlaylist(const QString &song, const QString &playlist) const noexcept {
    storage->addSongToPlaylist(song, playlist);
}

void DB::addPlaylist(const QString &playlistName, const songsVectorT &songs) const noexcept {
    storage->addPlaylist(playlistName, songs);
}

songsMapT* DB::getSongs() const noexcept {
    return storage->getSongs();
}

playlistMapT* DB::getPlaylists() const noexcept {
    return storage->getPlaylists();
}

void DB::removeSongs(const QStringList &absPathesToSongs) const noexcept{
    storage->deleteSongs(absPathesToSongs);
}

void DB::removePlaylist(const QString &playlistName) const noexcept {
    storage->deletePlaylist(playlistName);
}



