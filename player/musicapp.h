#ifndef MUSICAPP_H
#define MUSICAPP_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QListWidgetItem>
#include <QString>
#include "db.h"
#include "song.h"
#include "loop.h"
#include "shuffle.h"
#include "createplaylistform.h"

namespace Ui {
class MusicApp;
}

class MusicApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicApp(QWidget *parent = 0);
    ~MusicApp();

public slots:
    void on_addMusic_triggered() noexcept;
    void on_addPlaylist_triggered() noexcept;
    void onItemDBClicked(QListWidgetItem *item) noexcept;
    void onPlayPauseClick() noexcept;
    void onNextSongClick() noexcept;
    void onPrevSongClick() noexcept;
    void onLoopBtnClick() noexcept;
    void onShuffleBtnClick() noexcept;
    void onCurrentMediaChanged(int id) noexcept;
    void onSendPlaylist(std::pair<QString, std::vector<std::string>> received) noexcept;
    void onPlaylistClicked(QListWidgetItem* item) noexcept;
    void onPlaylistDBClicked(QListWidgetItem* item) noexcept;

private:
    bool isPlaying = false;
    int currentPlayingId = 0;
    std::unordered_map<std::string, std::shared_ptr<Song>> songsMap;
    std::unordered_map<std::string, std::vector<std::string>> playlistsMap;

    std::unique_ptr<Ui::MusicApp> ui;
    std::unique_ptr<QMediaPlayer> player;
    std::unique_ptr<QMediaPlaylist> playlist;
    std::unique_ptr<Loop> loop;
    std::unique_ptr<Shuffle> shuffle;
    std::unique_ptr<DB> dbPtr;
    std::unique_ptr<CreatePlaylistForm> form;

    void changeTitle() noexcept;
    void loadAllSongsToSongsMap(const int& id) noexcept;
    void reloadListOfSongsView() noexcept;
};

#endif // MUSICAPP_H
