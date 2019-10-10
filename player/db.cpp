#include "db.h"
#include "fs.h"
#include "statuses.h"
#include "storage.h"
#include "filestorage.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <QStringList>


DB::DB(bool &&stubMode) noexcept {
    std::cout << "open connection" << std::endl;
    if(stubMode) {
        storage = new FileStorage();
    } else {
        // open connection to real DB;
        // storage = new DBStorage();
    }
}

DB::~DB() noexcept {
    std::cout << "close connection" << std::endl;
    delete storage;
}

void DB::addSongsPathes(const std::vector<std::string>& songs) const noexcept {
    storage->insertSongsPathes(songs);
}

std::vector<std::string> DB::getSongsPathes() const noexcept {
    return storage->getSongsPathes();
}

void DB::removeSongs(const QStringList &absPathesToSongs) const noexcept{
    storage->deleteSongsPathes(absPathesToSongs);
}



