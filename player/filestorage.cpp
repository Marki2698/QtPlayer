#include "filestorage.h"
#include "fs.h"
#include "song.h"
#include "storage.h"

#include <fstream>
#include <vector>
#include <memory>
#include <QStringList>
#include <QDir>
#include <string>
#include <iostream>
#include <unordered_map>

void FileStorage::openFileStream(const std::string& fileName) noexcept {
    std::cout << fileName << std::endl;
    stubFile.open(fileName, std::ios::in | std::ios::out | std::ios::app);
}

void FileStorage::closeFileStream() noexcept {
    if (stubFile.is_open()) stubFile.close();
}

void FileStorage::insertSongsPathes(const std::vector<std::string>& songs) noexcept {
    openFileStream(songsFileName);
    for (const auto& path : songs) {
        stubFile << path << "\n";
    }
    closeFileStream();
}

void FileStorage::insertSongToPlaylist(const QString &song, const QString &playlist) noexcept {
    openFileStream(playlistsDir + "/" + playlist.toStdString());
    stubFile << song.toStdString() << std::endl;
    closeFileStream();
}

std::vector<std::string> FileStorage::getSongsPathes() noexcept {
    std::vector<std::string> pathes;
    std::string path;

    openFileStream(songsFileName);
    while(std::getline(stubFile, path)) {
        pathes.push_back(path);
        path.erase();
    }
    closeFileStream();

    return pathes;
}

std::unordered_map<std::string, std::vector<std::string>> FileStorage::getPlaylistsSongsPathes() noexcept {
    QDir dir(playlistsDir.c_str());
    QFileInfoList listOfFiles  = dir.entryInfoList(QDir::Files);

    std::unordered_map<std::string, std::vector<std::string>> playlistsMap;

    for (QFileInfo item : listOfFiles) {

        openFileStream((playlistsDir + "/" + item.fileName().toStdString()));
        std::string path;
        std::vector<std::string> pathes;

        while(std::getline(stubFile, path)) {
            pathes.emplace_back(path);
            path.erase();
        }
        closeFileStream();
        playlistsMap.insert({item.baseName().toStdString(), pathes});
    }

    return playlistsMap;


}

bool FileStorage::isEmpty(std::fstream &fileStream) const noexcept {
    return fileStream.peek() == fileStream.eof();
}

void FileStorage::deleteSongsPathes(const QStringList &absPathesToSongs) noexcept {

}
