#ifndef TYPES
#define TYPES

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <fstream>
#include <chrono>


#include <QDialog>
#include <QListWidget>
#include <QPixmap>
#include <QMediaPlaylist>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMediaPlayer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QMediaMetaData>
#include <QUrl>
#include <Qt>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "song.h"

//using namespace std;

using songsVectorT = std::vector<std::string>;
using playlistMapT = std::unordered_map<std::string, std::vector<std::string>>;
using playlistMapItemT = std::pair<std::string, std::vector<std::string>>;
using songsMapItemT = std::pair<std::string, std::shared_ptr<Song>>;

using songsMapT = std::unordered_map<std::string, std::shared_ptr<Song>>;

using playModeAndPixMapT = std::pair<QMediaPlaylist::PlaybackMode, QPixmap>;

#endif // TYPES

