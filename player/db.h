#ifndef DB_H
#define DB_H

#include "statuses.h"
#include "song.h"
#include "storage.h"
//#include "fs.h"

#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include <QStringList>


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
    void addSongsPathes(const std::vector<std::string>& songs) const noexcept;
    std::vector<std::string> getSongsPathes() const noexcept;
    std::unordered_map<std::string, std::vector<std::string>> getPlaylistsSongsPathes() const noexcept;
    void removeSongs(const QStringList& absPathesToSongs) const noexcept;
private:
    std::unique_ptr<AbstractStorage> storage;
};

#endif // DB_H
