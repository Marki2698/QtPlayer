#ifndef STORAGE_H
#define STORAGE_H

#include "types.h"

#include "song.h"


class AbstractStorage {
public:
    AbstractStorage() {}
    virtual ~AbstractStorage() {}
    virtual void insertSongsPathes(const std::vector<std::string> &songs) = 0;
    virtual void insertSongToPlaylist(const QString& song, const QString& playlist) = 0;
    virtual std::vector<std::string> getSongsPathes() = 0;
    virtual std::unordered_map<std::string, std::vector<std::string>> getPlaylistsSongsPathes() = 0;
    virtual void deleteSongsPathes(const QStringList& absPathesToSongs) = 0;
};

#endif // STORAGE_H
