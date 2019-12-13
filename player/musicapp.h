#ifndef MUSICAPP_H
#define MUSICAPP_H

#include "types.h"

#include "db.h"
#include "song.h"
#include "loop.h"
#include "shuffle.h"
#include "createplaylistform.h"
#include "removesongsform.h"

#include <QFileSystemWatcher>

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
    void on_removeSongsGlobally_triggered(bool triggered) noexcept;
    void onSongDBClicked(QListWidgetItem *item) noexcept;
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
    void addSongToPlaylist(const QString &song, const QString &playlist) noexcept;
    void onRemovePlaylistClicked(bool triggered) noexcept;
    void onPlaylistSongDBClicked(QListWidgetItem* item) noexcept;
    void onNotificationFromRemoveForm() noexcept;
    void onRemoveOneSongGlobally(bool triggered) noexcept;

private:
    bool isPlaying = false;
    int currentPlayingId = 0;
    std::string activePlaylist = "";

    songsMapT * songsMap;
    playlistMapT * playlistsMap;

    std::unique_ptr<Ui::MusicApp> ui;
    std::unique_ptr<QMediaPlayer> player;
    std::unique_ptr<QMediaPlaylist> playlist;
    std::unique_ptr<Loop> loop;
    std::unique_ptr<Shuffle> shuffle;
    std::shared_ptr<DB> dbPtr;
    std::unique_ptr<CreatePlaylistForm> createPlaylistForm;
    std::unique_ptr<RemoveSongsForm> removeSongsForm;
    std::shared_ptr<QMenu> mainMenu;
    std::shared_ptr<QMenu> listOfPlaylistMenu;
    std::shared_ptr<QMenu> playlistMenu;
    std::shared_ptr<QMenu> songInPlaylistMenu;
    QList<QAction*> listOfSongsMenuActions;
    QList<QAction*> playlistsActions;


    void changeTitle() noexcept;
    void loadAllSongsToPlayer(const int& id) noexcept;
    void reloadListOfSongsView() noexcept;
    void reloadListOfPlaylistSongsView(const std::string& playlist = "") noexcept;
    void addPlaylistsToMenuActions(const std::string &oneItem = "") noexcept;
    void setPlaylistForPlayer(const std::string& playlistName) noexcept;
    inline QList<QAction*> buildListOfActions(const std::string& item) noexcept;
    inline QPixmap setPlayingIcon(const bool& isPlayling) noexcept;
    inline void setActivePlaylist(const std::string& name) noexcept;
    inline void setPlayingStatus(bool&& status) noexcept;
    inline void setCurrentPlayIndex(const int& id) noexcept;
    inline void setIconAndPlay() noexcept;

};

#endif // MUSICAPP_H
