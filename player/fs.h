#ifndef FS_H
#define FS_H


#include <string>
#include <QStringList>
#include "musicapp.h"

const std::string STUB_FILE_PATH = "stub.txt";

class fs
{
public:
    fs() {}
    ~fs() {}
    static QStringList chooseSongsToAdd(MusicApp* parent) noexcept;
    static void createUTF8File(const std::string filename);
};

#endif // FS_H
