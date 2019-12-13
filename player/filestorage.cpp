#include "filestorage.h"
#include "fs.h"
#include "song.h"
#include "storage.h"
#include "types.h"

#include <cstdio>
#include <algorithm>

// here filestorage have only to flush changes to files and only
// maybe remove idea with db and make filestorage primary storage

void FileStorage::openFileStream(const std::string& fileName) noexcept {
    std::cout << fileName << std::endl;
    sourceFile.open(fileName, std::ios::in | std::ios::out | std::ios::app);
}

void FileStorage::closeFileStream() noexcept {
    if (sourceFile.is_open()) sourceFile.close();
}

void FileStorage::addSongs(const songsVectorT &songs) noexcept {
    songsMapT newSongsMap = Song::getSongsMap(songs);
    songsMap.insert(std::make_move_iterator(newSongsMap.begin()),
                    std::make_move_iterator(newSongsMap.end()));

    // to separate method flush or something;
    openFileStream(songsFileName);
    for (const auto& path : songs) {
        sourceFile << path << "\n";
    }
    closeFileStream();
}

void FileStorage::addSongToPlaylist(const QString &song, const QString &playlist) noexcept {
    auto playlistToModify = playlistsMap.find(playlist.toStdString());
    playlistToModify->second.emplace_back(song.toStdString());

    // to separate method flush or something;
    openFileStream(playlistsDir + "/" + playlist.toStdString());
    sourceFile << song.toStdString() << "\n";
    closeFileStream();

    // add song to playlistMap
}

void FileStorage::addPlaylist(const QString &playlistName, const songsVectorT &songs) noexcept {
    playlistsMap.insert({playlistName.toStdString(), songs});

    // to separate method flush or something;

    fs::createUTF8File(playlistsDir + "/" + playlistName.toStdString());
    openFileStream(playlistsDir + "/" + playlistName.toStdString());
    for (const auto& song: songs) sourceFile << song << "\n";
    closeFileStream();
}

songsMapT* FileStorage::getSongs() noexcept {
    std::vector<std::string> pathes;
    std::string path;

    openFileStream(songsFileName);
    while(std::getline(sourceFile, path)) {
        pathes.push_back(path);
        path.erase();
    }
    closeFileStream();

    songsMap = Song::getSongsMap(pathes);

    return &songsMap;
}

playlistMapT* FileStorage::getPlaylists() noexcept {
    QDir dir(playlistsDir.c_str());
    QFileInfoList listOfFiles  = dir.entryInfoList(QDir::Files);

    for (QFileInfo item : listOfFiles) {

        openFileStream((playlistsDir + "/" + item.fileName().toStdString()));
        std::string path;
        songsVectorT pathes;

        while(std::getline(sourceFile, path)) {
            pathes.emplace_back(path);
            path.erase();
        }
        closeFileStream();
        playlistsMap.insert({item.baseName().toStdString(), pathes});
    }

    return &playlistsMap;
}

bool FileStorage::isEmpty(std::fstream &fileStream) const noexcept {
    return fileStream.peek() == fileStream.eof();
}

void FileStorage::deleteSongs(const QStringList &songs) noexcept {
    //erasing from maps

    for (const QString& QSong: songs) {
        songsMap.erase(QSong.toStdString());

        for (auto playlist = playlistsMap.begin(); playlist != playlistsMap.end(); ++playlist) {
            auto iter = std::find((*playlist).second.begin(), (*playlist).second.end(), QSong.toStdString());
            if (iter != (*playlist).second.end()) (*playlist).second.erase(iter);
        }
    }



    // remove on disk
    // open file with trunc modifier and save
    // songs file

    // for songs file
    sourceFile.open(SONGS_FILE_PATH,std::ios::out | std::ios::trunc);
    for (const songsMapItemT& song: songsMap) {
        sourceFile << song.second->getStdPath() << "\n";
    }
    sourceFile.close();


    // for playlist files
    for (const playlistMapItemT& playlist : playlistsMap) {
        sourceFile.open(PLAYLIST_DIR_PATH + "/" + playlist.first,std::ios::out | std::ios::trunc);
        for (const std::string& song: playlist.second) {
            sourceFile << song << "\n";
        }
        sourceFile.close();
    }
}

void FileStorage::deletePlaylist(const QString &playlistName) noexcept {
    playlistsMap.erase(playlistName.toStdString());

    std::remove((playlistsDir + "/" + playlistName.toStdString()).c_str());
}
