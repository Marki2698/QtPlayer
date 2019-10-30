#ifndef TYPES
#define TYPES

#include <unordered_map>
#include <string>
#include <vector>

#include <QPixmap>
#include <QMediaPlaylist>

using namespace std;

using songsVectorT = vector<string>;
using playlistMapT = unordered_map<string, vector<string>>; // why not use shared ptr's instead of string here?
using playlistMapItemT = std::pair<std::string, std::vector<std::string>>;

template <class T = vector<string>>
using songsMapT = unordered_map<string, T>;

using playModeAndPixMapT = pair<QMediaPlaylist::PlaybackMode, QPixmap>;

#endif // TYPES

