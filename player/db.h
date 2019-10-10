#ifndef DB_H
#define DB_H

#include "statuses.h"
#include "song.h"
#include "storage.h"

#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>

#include <QStringList>

//const bool STUB_MODE = true;


class DB
{
public:
    DB(bool&& stubMode) noexcept;
    DB(const DB& source) = delete;
    DB& operator= (const DB& source) = delete;
    DB(const DB&& source) = delete;
    DB& operator= (const DB&& source) = delete;
    ~DB() noexcept;
    void addSongsPathes(const std::vector<std::string>& songs) const noexcept;
    std::vector<std::string> getSongsPathes() const noexcept;
    void removeSongs(const QStringList& absPathesToSongs) const noexcept;
private:
    AbstractStorage* storage = nullptr;
};

#endif // DB_H
