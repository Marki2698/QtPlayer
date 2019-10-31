#ifndef TYPES
#define TYPES

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <fstream>


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

using namespace std;

using songsVectorT = vector<string>;
using playlistMapT = unordered_map<string, vector<string>>; // why not use shared ptr's instead of string here?
using playlistMapItemT = std::pair<std::string, std::vector<std::string>>;

template <class T = vector<string>>
using songsMapT = unordered_map<string, T>;

using playModeAndPixMapT = pair<QMediaPlaylist::PlaybackMode, QPixmap>;

#endif // TYPES

