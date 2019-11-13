#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "types.h"
#include "song.h"

#include "storage.h"


// add copies of songsMap and playlistsMap reflect both versions

class FileStorage: public AbstractStorage {
public:
    FileStorage(std::string songFile, std::string playDir): songsFileName(songFile), playlistsDir(playDir) {}
    ~FileStorage() {}
    void addSongs(const songsVectorT &songs) noexcept;
    void addSongToPlaylist(const QString& song, const QString& playlist) noexcept;
    void addPlaylist(const QString& playlistName, const songsVectorT& songs) noexcept;
    songsMapT* getSongs() noexcept;
    playlistMapT* getPlaylists() noexcept;
    void deleteSongs(const QStringList &songs) noexcept;
    void deletePlaylist(const QString& playlistName) noexcept;
    bool isEmpty(std::fstream& fileStream) const noexcept;

private:
    std::fstream sourceFile;
    std::string songsFileName;
    std::string playlistsDir;
    songsMapT songsMap;
    playlistMapT playlistsMap;
    void openFileStream(const std::string& fileName) noexcept;
    void closeFileStream() noexcept;
};

#endif // FILESTORAGE_H
