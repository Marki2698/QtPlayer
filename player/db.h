#ifndef DB_H
#define DB_H

#include "statuses.h"
#include <QStringList>

const bool STUB_MODE = true;


class db
{
public:
    db() noexcept;
    db(const db& source) = delete;
    db& operator= (const db& source) = delete;
    db(const db&& source) = delete;
    db& operator= (const db&& source) = delete;
    Status addSongs(const QStringList& absPathesToSongs) const noexcept;
    Status removeSongs(const QStringList& absPathesToSongs) const noexcept;
};

#endif // DB_H
