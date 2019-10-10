#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "storage.h"

#include <QStringList>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_map>


class FileStorage: public AbstractStorage {
public:
    FileStorage() {}
    ~FileStorage() {}
    void insertSongsPathes(const std::vector<std::string> &songs) noexcept;
    std::vector<std::string> getSongsPathes() noexcept;
    void deleteSongsPathes(const QStringList &absPathesToSongs) noexcept;
    bool isEmpty(std::fstream& fileStream) const noexcept;

private:
    std::fstream stubFile;
    void openFileStream() noexcept;
    void closeFileStream() noexcept;
};

#endif // FILESTORAGE_H
