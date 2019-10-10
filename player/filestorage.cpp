#include "filestorage.h"
#include "fs.h"
#include "song.h"
#include "storage.h"

#include <fstream>
#include <vector>
#include <memory>
#include <QStringList>
#include <string>
#include <iostream>
#include <unordered_map>

void FileStorage::openFileStream() noexcept {
    stubFile.open(STUB_FILE_PATH, std::ios::in | std::ios::out | std::ios::app);
}

void FileStorage::closeFileStream() noexcept {
    if (stubFile.is_open()) stubFile.close();
}

void FileStorage::insertSongsPathes(const std::vector<std::string>& songs) noexcept {
    openFileStream();
    for (const auto& path : songs) {
        stubFile << path << std::endl;
    }
    closeFileStream();
}

std::vector<std::string> FileStorage::getSongsPathes() noexcept {
    std::vector<std::string> pathes;
    std::string path;

    openFileStream();
    while(std::getline(stubFile, path)) {
        pathes.push_back(path);
        path.erase();
    }
    closeFileStream();

    return pathes;
}

bool FileStorage::isEmpty(std::fstream &fileStream) const noexcept {
    return fileStream.peek() == fileStream.eof();
}

void FileStorage::deleteSongsPathes(const QStringList &absPathesToSongs) noexcept {

}
