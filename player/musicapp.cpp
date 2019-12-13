#include "types.h"

#include "musicapp.h"
#include "ui_musicapp.h"
#include "fs.h"
#include "song.h"
#include "db.h"
#include "loop.h"
#include "shuffle.h"
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/audioproperties.h"
#include "utils.h"
#include "createplaylistform.h"
#include "menubuilder.h"


MusicApp::MusicApp(QWidget *parent) :
    QMainWindow(parent),
    ui(make_unique<Ui::MusicApp>()),
    player(make_unique<QMediaPlayer>()),
    loop(make_unique<Loop>()),
    shuffle(make_unique<Shuffle>()),
    dbPtr(std::make_shared<DB>(SONGS_FILE_PATH, PLAYLIST_DIR_PATH)),
    playlist(make_unique<QMediaPlaylist>())
{
    fs::createUTF8File(SONGS_FILE_PATH);
    fs::createPlaylistFolder(PLAYLIST_DIR_PATH);

    mainMenu = make_unique<QMenu>(this);
    playlistMenu = make_unique<QMenu>(this);

    // move somewhere
    QAction* a = new QAction(QString("remove"), playlistMenu.get());
    playlistsActions.append(a);
    playlistMenu->addActions(playlistsActions);
    connect(a, SIGNAL(triggered(bool)), this, SLOT(onRemovePlaylistClicked(bool)));


    ui->setupUi(this);
    listOfPlaylistMenu = MenuBuilder::createSubMenu(mainMenu, "Add to playlist");
    QList<QAction*> actionsList = MenuBuilder::addActionsToMenu(mainMenu, "remove");
    connect(actionsList.first(), SIGNAL(triggered(bool)), this, SLOT(onRemoveOneSongGlobally(bool)));

    songsMap = dbPtr->getSongs();
    playlistsMap = dbPtr->getPlaylists();


    QStringList listOfPathes;
    for (const songsMapItemT& song : *songsMap) listOfPathes.push_back(song.second->getQStrPath());


    for (const auto& playlist : *playlistsMap) {
        ui->listOfPlaylists->addItem(QString(playlist.first.c_str()));
    }

    addPlaylistsToMenuActions();

    player->setPlaylist(playlist.get());
    loadAllSongsToPlayer(player->playlist()->currentIndex());


    // to separate function
    ui->listOfSongs->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listOfPlaylists->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listOfSongsInPlaylist->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(ui->listOfSongs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onSongDBClicked(QListWidgetItem*)));
    connect(ui->listOfSongs, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClick(QPoint)));
    connect(ui->playAndPause, SIGNAL(clicked()), this, SLOT(onPlayPauseClick()));
    connect(ui->nextSong, SIGNAL(clicked()), this, SLOT(onNextSongClick()));
    connect(ui->prevSong, SIGNAL(clicked()), this, SLOT(onPrevSongClick()));
    connect(ui->loopBtn, SIGNAL(clicked()), this, SLOT(onLoopBtnClick()));
    connect(ui->shuffleBtn, SIGNAL(clicked()), this, SLOT(onShuffleBtnClick()));
    connect(playlist.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentMediaChanged(int)));
    connect(ui->listOfPlaylists, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onPlaylistClicked(QListWidgetItem*)));
    connect(ui->listOfPlaylists, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onPlaylistDBClicked(QListWidgetItem*)));
    connect(ui->listOfSongsInPlaylist, SIGNAL(itemDoubleClicked(QListWidgetItem*)) ,this, SLOT(onPlaylistSongDBClicked(QListWidgetItem*)));
    connect(ui->listOfPlaylists , SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClick(QPoint)));
}

void MusicApp::changeTitle() noexcept {
    ui->title->setText(player->metaData(QMediaMetaData::Title).toString());
}

void MusicApp::loadAllSongsToPlayer(const int& id) noexcept {
    for (const auto& song : *songsMap) {
        playlist->addMedia(QUrl::fromLocalFile(song.second->getQStrPath())); // performance is weak
    }

    if (ui->listOfSongs->count() == 0) {
        for (const auto& song : *songsMap) ui->listOfSongs->addItem(song.second->show());
    }

    player->playlist()->setCurrentIndex(id);
}

void MusicApp::onSongDBClicked(QListWidgetItem* item) noexcept {
    player->playlist()->clear();
    std::cout << ui->listOfSongs->row(item) << std::endl;
    loadAllSongsToPlayer(ui->listOfSongs->row(item));

    player->play();

    ui->playAndPause->setIcon(QPixmap(":/recources/images/pause.png"));
    changeTitle();
    activePlaylist = "";
}

void MusicApp::onRightClick(QPoint point) noexcept {
    if (ui->tabs->currentIndex() == 0) {
        mainMenu->popup(ui->listOfSongs->viewport()->mapToGlobal(point));
    } else {
        playlistMenu->popup(ui->listOfPlaylists->viewport()->mapToGlobal(point));
    }
}


void MusicApp::addSongToPlaylist(const QString& song, const QString& playlist) noexcept {
    std::string playlistName = playlist.toStdString();

    dbPtr->addSongToPlaylist(song, playlist);

    if (activePlaylist == playlistName) {
        reloadListOfPlaylistSongsView(playlistName);
    }
}

void MusicApp::onAddToClicked(QString text) noexcept {
    QString song = ui->listOfSongs->selectedItems().first()->text();
    addSongToPlaylist(song, text);

    reloadListOfPlaylistSongsView(text.toStdString());
}

void MusicApp::onPlayPauseClick() noexcept {
    isPlaying = !isPlaying;

    if (isPlaying) player->play();
    else player->pause();

    ui->playAndPause->setIcon(setPlayingIcon(isPlaying));
}

inline QPixmap MusicApp::setPlayingIcon(const bool& isPlayling) noexcept {
    if (isPlayling) return QPixmap(":/recources/images/pause.png");
    else return QPixmap(":/recources/images/play.png");
}

void MusicApp::onNextSongClick() noexcept {
    if (player->playlist()->currentIndex() == player->playlist()->mediaCount() - 1 &&
            loop->getCurrentLoopMode().first == QMediaPlaylist::PlaybackMode::Sequential) {
        setPlayingStatus(false);

        player->stop();
        player->playlist()->setCurrentIndex(0);
    } else {
        player->playlist()->next();
    }

    changeTitle();
}

void MusicApp::onPrevSongClick() noexcept {
    if (player->playlist()->currentIndex() == 0 &&
            loop->getCurrentLoopMode().first == QMediaPlaylist::PlaybackMode::Sequential) {
        setPlayingStatus(false);

        player->stop();
    } else {
        player->playlist()->previous();
    }

    changeTitle();
}

void MusicApp::onLoopBtnClick() noexcept {
    loop->nextLoopMode();
    player->playlist()->setPlaybackMode(loop->getCurrentLoopMode().first);
    ui->loopBtn->setIcon(loop->getCurrentLoopMode().second);
}

void MusicApp::onShuffleBtnClick() noexcept {
    auto shuffleMode = shuffle->nextShuffleMode();
    player->playlist()->setPlaybackMode(shuffleMode.first);
    ui->shuffleBtn->setIcon(shuffleMode.second);
}

void MusicApp::onCurrentMediaChanged(int id) noexcept {
    ui->listOfSongs->item(currentPlayingId)->setSelected(false);
    currentPlayingId = id;
    std::cout << currentPlayingId << std::endl;
    if (currentPlayingId == -1) {
        setPlayingStatus(false);
    } else {
        ui->listOfSongs->item(currentPlayingId)->setSelected(true);
    }
}

void MusicApp::reloadListOfSongsView() noexcept {
    ui->listOfSongs->clear();
    for (auto& song : *songsMap) {
        ui->listOfSongs->addItem(song.second->show());
    }
}

void MusicApp::reloadListOfPlaylistSongsView(const std::string &playlist) noexcept {
    ui->listOfSongsInPlaylist->clear();
    auto listOfSelectedPlaylists = ui->listOfPlaylists->selectedItems();
    if (!listOfSelectedPlaylists.empty()) {
        auto occurancy = playlistsMap->find(listOfSelectedPlaylists.first()->text().toStdString());
        for (auto& song : occurancy->second) {
            ui->listOfSongsInPlaylist->addItem(QString(song.c_str()));
        }
    }
}

void MusicApp::onRemovePlaylistClicked(bool triggered) noexcept {
    auto selectedPlaylist = ui->listOfPlaylists->selectedItems().first();
    QString playlistToRemove = selectedPlaylist->text();
    dbPtr->removePlaylist(playlistToRemove);


    qDeleteAll(ui->listOfPlaylists->selectedItems());
    ui->listOfSongsInPlaylist->clear();

    ui->playAndPause->setIcon(setPlayingIcon(false));

    if (activePlaylist == playlistToRemove.toStdString()) {
        player->playlist()->clear();
        loadAllSongsToPlayer(0);
    }

    for (auto iter = listOfSongsMenuActions.begin() ;iter != listOfSongsMenuActions.end(); ++iter) {
        if ((*iter)->text() == playlistToRemove) {
            listOfPlaylistMenu->removeAction(*iter);
            break;
        }
    }

    if (playlistsMap->empty()) {
        mainMenu->removeAction(listOfPlaylistMenu->menuAction());
    }
}

void MusicApp::addPlaylistsToMenuActions(const std::string& oneItem) noexcept {
    QList<QAction*> listOfActions = buildListOfActions(oneItem);
    listOfSongsMenuActions.append(listOfActions);

    for (QAction* playlistAction : listOfActions) {
        QSignalMapper* mapper = new QSignalMapper(this);
        connect(playlistAction, SIGNAL(triggered(bool)), mapper, SLOT(map()));

        mapper->setMapping(playlistAction, playlistAction->text());

        connect(mapper, SIGNAL(mapped(QString)), this, SLOT(onAddToClicked(QString)));
    }
}

inline QList<QAction*> MusicApp::buildListOfActions(const std::string &item) noexcept {
    if (item.empty()) return MenuBuilder::addActionsToMenu(listOfPlaylistMenu, *playlistsMap);
    else return MenuBuilder::addActionsToMenu(listOfPlaylistMenu, item);
}

void MusicApp::on_addMusic_triggered() noexcept {
    QStringList songAbsPathes = fs::chooseSongsToAdd(this);

    if (!songAbsPathes.isEmpty()) {
        songsVectorT songsPathes;
        for (QString& path : songAbsPathes) {
            songsPathes.emplace_back(path.toStdString());
        }

        dbPtr->addSongs(songsPathes);

        reloadListOfSongsView(); // bad performance

    }
}

void MusicApp::on_addPlaylist_triggered() noexcept {
    createPlaylistForm = make_unique<CreatePlaylistForm>(ui->listOfSongs, dbPtr);
    connect(createPlaylistForm.get(),
            SIGNAL(sendPlaylistToMain(std::pair<QString, songsVectorT>)),
            this,
            SLOT(onSendPlaylist(std::pair<QString, songsVectorT >)));

    createPlaylistForm->show();
}

inline void MusicApp::setActivePlaylist(const std::string &name) noexcept {
    activePlaylist = name;
}

inline void MusicApp::setPlayingStatus(bool &&status) noexcept {
    isPlaying = status;
    setIconAndPlay();
}

inline void MusicApp::setCurrentPlayIndex(const int &id) noexcept {
    player->playlist()->setCurrentIndex(id);
}

inline void MusicApp::setIconAndPlay() noexcept {
    ui->playAndPause->setIcon(setPlayingIcon(isPlaying));
    player->play();
}

void MusicApp::onSendPlaylist(std::pair<QString, songsVectorT > received) noexcept {
    ui->listOfPlaylists->addItem(received.first);

    playlistMapItemT item {received.first.toStdString(), received.second};

    addPlaylistsToMenuActions(item.first);
}

void MusicApp::onPlaylistClicked(QListWidgetItem *item) noexcept {
    auto occurancy = playlistsMap->find(item->text().toStdString());
    ui->listOfSongsInPlaylist->clear();
    for (auto label : occurancy->second) {
        ui->listOfSongsInPlaylist->addItem(QString(label.c_str()));
    }
}

void MusicApp::onPlaylistDBClicked(QListWidgetItem *item) noexcept {
    std::string playlistName = item->text().toStdString();

    setActivePlaylist(playlistName);

    setPlaylistForPlayer(playlistName);

    setPlayingStatus(true);

    setIconAndPlay();
}

void MusicApp::setPlaylistForPlayer(const std::string &playlistName) noexcept {
    songsVectorT songs = playlistsMap->find(playlistName)->second;

    player->playlist()->clear();

    for (auto& song : songs) {
        auto songData = songsMap->find(song)->second;
        playlist->addMedia(QUrl::fromLocalFile(songData->getQStrPath()));
    }
}

void MusicApp::onPlaylistSongDBClicked(QListWidgetItem *item) noexcept {
    std::string playlistName = ui->listOfPlaylists->selectedItems().first()->text().toStdString();
    setPlayingStatus(true);

    if (activePlaylist != playlistName) {
        setPlaylistForPlayer(playlistName);

        player->playlist()->clear();

        setPlaylistForPlayer(playlistName);
    }

    setCurrentPlayIndex(ui->listOfSongsInPlaylist->row(item));
    setIconAndPlay();

}

void MusicApp::onRemoveOneSongGlobally(bool triggered) noexcept {
    dbPtr->removeSongs(QStringList(ui->listOfSongs->selectedItems().first()->text()));
    reloadListOfSongsView();

    reloadListOfPlaylistSongsView();
}

void MusicApp::on_removeSongsGlobally_triggered(bool triggered) noexcept {
    removeSongsForm = make_unique<RemoveSongsForm>(ui->listOfSongs, dbPtr);
    connect(removeSongsForm.get(),
            SIGNAL(notifyMainWindow()),
            this, SLOT(onNotificationFromRemoveForm()));

    removeSongsForm->show();
}

void MusicApp::onNotificationFromRemoveForm() noexcept {
    reloadListOfSongsView();

    reloadListOfPlaylistSongsView();
}

MusicApp::~MusicApp() {}
