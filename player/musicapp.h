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
#include <QPoint>
#include <QMenu>
#include "db.h"
#include "song.h"
#include "loop.h"
#include "shuffle.h"
#include "createplaylistform.h"
#include "types.h"

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
    void onSendPlaylist(std::pair<QString, songsVectorT> received) noexcept;
    void onPlaylistClicked(QListWidgetItem* item) noexcept;
    void onPlaylistDBClicked(QListWidgetItem* item) noexcept;
    void onRightClick(QPoint point) noexcept;
    void onAddToClicked(QString text) noexcept;
    void addSongToPlaylist(QString song, QString playlist) noexcept;

private:
    bool isPlaying = false;
    int currentPlayingId = 0;
    std::string activePlaylist = "";

    songsMapT<std::shared_ptr<Song>> songsMap;
    playlistMapT playlistsMap;

    std::unique_ptr<Ui::MusicApp> ui;
    std::unique_ptr<QMediaPlayer> player;
    std::unique_ptr<QMediaPlaylist> playlist;
    std::unique_ptr<Loop> loop;
    std::unique_ptr<Shuffle> shuffle;
    std::unique_ptr<DB> dbPtr;
    std::unique_ptr<CreatePlaylistForm> form;
    std::shared_ptr<QMenu> mainMenu;
    std::shared_ptr<QMenu> listOfPlaylistMenu;

    void changeTitle() noexcept;
    void loadAllSongsToSongsMap(const int& id) noexcept;
    void reloadListOfSongsView() noexcept;
    void reloadListOfPlaylistSongsView(const std::string& playlist) noexcept;
    void addPlaylistsToMenuActions(const std::string &oneItem = "") noexcept;
    inline QList<QAction*> buildListOfActions(const std::string& item) noexcept;
    inline QPixmap setPlayingIcon(const bool& isPlayling) noexcept;

};

#endif // MUSICAPP_H
