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
//    static writeData();
    static std::string readData(const std::string& filePath = STUB_FILE_PATH);
    static QStringList chooseSongsToAdd(MusicApp* parent) noexcept;
};

#endif // FS_H
