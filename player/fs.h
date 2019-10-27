#ifndef FS_H
#define FS_H


#include <string>
#include <QStringList>
#include "musicapp.h"

const std::string STUB_FILE_PATH = "songs";
const std::string PLAYLIST_DIR_PATH = "playlists";

class fs
{
public:
    fs() {}
    ~fs() {}
    static QStringList chooseSongsToAdd(MusicApp* parent) noexcept;
    static void createUTF8File(const std::string& filename);
    static void createPlaylistFolder(const std::string& dirname);
};

#endif // FS_H
