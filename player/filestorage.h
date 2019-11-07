#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "types.h"

#include "storage.h"


class FileStorage: public AbstractStorage {
public:
    FileStorage(std::string songFile, std::string playDir): songsFileName(songFile), playlistsDir(playDir) {}
    ~FileStorage() {}
    void insertSongsPathes(const songsVectorT &songs) noexcept;
    void insertSongToPlaylist(const QString& song, const QString& playlist) noexcept;
    songsVectorT getSongsPathes() noexcept;
    playlistMapT getPlaylistsSongsPathes() noexcept;
    void deleteSongsPathes(const QStringList &absPathesToSongs) noexcept;
    void deletePlaylistPathes(const QString& playlistName) noexcept;
    bool isEmpty(std::fstream& fileStream) const noexcept;

private:
    std::fstream stubFile;
    std::string songsFileName;
    std::string playlistsDir;
    void openFileStream(const std::string& fileName) noexcept;
    void closeFileStream() noexcept;
};

#endif // FILESTORAGE_H
