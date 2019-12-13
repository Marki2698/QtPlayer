#ifndef DB_H
#define DB_H


#include "types.h"

#include "statuses.h"
#include "song.h"
#include "storage.h"

class DB
{
public:
    static const bool stubMode = true;

    DB() noexcept;
    DB(const std::string& filename, const std::string& playlistDir) noexcept;
    DB(const DB& source) = delete;
    DB& operator= (const DB& source) = delete;
    DB(const DB&& source) = delete;
    DB& operator= (const DB&& source) = delete;
    ~DB() noexcept;

    void addSongs(const songsVectorT& songs) const noexcept;
    void addSongToPlaylist(const QString& song, const QString& playlist) const noexcept;
    void addPlaylist(const QString& playlistName, const songsVectorT& songs) const noexcept;
    songsMapT* getSongs() const noexcept;
    playlistMapT* getPlaylists() const noexcept;
    void removeSongs(const QStringList& absPathesToSongs) const noexcept;
    void removePlaylist(const QString& playlistName) const noexcept;
private:
    std::unique_ptr<AbstractStorage> storage;
    songsMapT songsMap;
    playlistMapT playlistsMap;
};

#endif // DB_H
