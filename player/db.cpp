#include "db.h"
#include "fs.h"
#include "statuses.h"
#include "storage.h"
#include "filestorage.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <QStringList>


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

// rename to addSongs
void DB::addSongsPathes(const std::vector<std::string>& songs) const noexcept {
    storage->insertSongsPathes(songs);
}

std::vector<std::string> DB::getSongsPathes() const noexcept {
    return storage->getSongsPathes();
}

std::unordered_map<std::string, std::vector<std::string>> DB::getPlaylistsSongsPathes() const noexcept {
    return storage->getPlaylistsSongsPathes();
}

void DB::removeSongs(const QStringList &absPathesToSongs) const noexcept{
    storage->deleteSongsPathes(absPathesToSongs);
}



