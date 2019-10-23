#ifndef STORAGE_H
#define STORAGE_H

#include "song.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include<QStringList>

class AbstractStorage {
public:
    AbstractStorage() {}
    virtual ~AbstractStorage() {}
    virtual void insertSongsPathes(const std::vector<std::string> &songs) = 0;
    virtual std::vector<std::string> getSongsPathes() = 0;
    virtual std::unordered_map<std::string, std::vector<std::string>> getPlaylistsSongsPathes() = 0;
    virtual void deleteSongsPathes(const QStringList& absPathesToSongs) = 0;
};

#endif // STORAGE_H
