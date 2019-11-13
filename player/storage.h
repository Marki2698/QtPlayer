#ifndef STORAGE_H
#define STORAGE_H

#include "types.h"

#include "song.h"


class AbstractStorage {
public:
    AbstractStorage() {}
    virtual ~AbstractStorage() {}
    virtual void addSongs(const songsVectorT &songs) = 0;
    virtual void addPlaylist(const QString& playlistName, const songsVectorT& songs) = 0;
    virtual void addSongToPlaylist(const QString& song, const QString& playlist) = 0;
    virtual songsMapT* getSongs() = 0;
    virtual playlistMapT* getPlaylists() = 0;
    virtual void deleteSongs(const QStringList& absPathesToSongs) = 0;
    virtual void deletePlaylist(const QString& playlistName) = 0;
};

#endif // STORAGE_H
