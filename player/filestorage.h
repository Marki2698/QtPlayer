#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "storage.h"

#include <QStringList>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <unordered_map>


class FileStorage: public AbstractStorage {
public:
    FileStorage(std::string songFile, std::string playDir): songsFileName(songFile), playlistsDir(playDir) {}
    ~FileStorage() {}
    void insertSongsPathes(const std::vector<std::string> &songs) noexcept;
    void insertSongToPlaylist(const QString& song, const QString& playlist) noexcept;
    std::vector<std::string> getSongsPathes() noexcept;
    std::unordered_map<std::string, std::vector<std::string>> getPlaylistsSongsPathes() noexcept;
    void deleteSongsPathes(const QStringList &absPathesToSongs) noexcept;
    bool isEmpty(std::fstream& fileStream) const noexcept;

private:
    std::fstream stubFile;
    std::string songsFileName;
    std::string playlistsDir;
    void openFileStream(const std::string& fileName) noexcept;
    void closeFileStream() noexcept;
};

#endif // FILESTORAGE_H
