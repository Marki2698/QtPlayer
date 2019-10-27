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

#include <iostream>
#include <memory>
#include <utility>
#include <fstream>
#include <string>

#include <QString>
#include <QStringList>
#include <QList>
#include <QMediaPlayer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QUrl>
#include <QPixmap>
#include <Qt>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>


MusicApp::MusicApp(QWidget *parent) :
    QMainWindow(parent),
    ui(make_unique<Ui::MusicApp>()),
    player(make_unique<QMediaPlayer>()),
    loop(make_unique<Loop>()),
    shuffle(make_unique<Shuffle>()),
    dbPtr(make_unique<DB>(STUB_FILE_PATH, PLAYLIST_DIR_PATH)),
    mainMenu(make_unique<QMenu>(this)),
    playlist(make_unique<QMediaPlaylist>())
{
    fs::createUTF8File(STUB_FILE_PATH);
    fs::createPlaylistFolder(PLAYLIST_DIR_PATH);


    ui->setupUi(this);
    listOfPlaylistMenu = MenuBuilder::createSubMenu(mainMenu, "Add to playlist");

    songsMap = Song::getSongsMap(dbPtr->getSongsPathes());
    playlistsMap = dbPtr->getPlaylistsSongsPathes();

    for (const auto& playlist : playlistsMap) {
        ui->listOfPlaylists->addItem(QString(playlist.first.c_str()));
    }

    // move to separate method and call every time I create new playlist
    addPlaylistsToMenuActions();

    player->setPlaylist(playlist.get());
    loadAllSongsToSongsMap(player->playlist()->currentIndex());


    ui->listOfSongs->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(ui->listOfSongs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDBClicked(QListWidgetItem*)));
    connect(ui->listOfSongs, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRightClick(QPoint)));
    connect(ui->playAndPause, SIGNAL(clicked()), this, SLOT(onPlayPauseClick()));
    connect(ui->nextSong, SIGNAL(clicked()), this, SLOT(onNextSongClick()));
    connect(ui->prevSong, SIGNAL(clicked()), this, SLOT(onPrevSongClick()));
    connect(ui->loopBtn, SIGNAL(clicked()), this, SLOT(onLoopBtnClick()));
    connect(ui->shuffleBtn, SIGNAL(clicked()), this, SLOT(onShuffleBtnClick()));
    connect(playlist.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentMediaChanged(int)));
    connect(ui->listOfPlaylists, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onPlaylistClicked(QListWidgetItem*)));
    connect(ui->listOfPlaylists, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onPlaylistDBClicked(QListWidgetItem*)));
}

void MusicApp::changeTitle() noexcept {
    ui->title->setText(player->metaData(QMediaMetaData::Title).toString());
}

void MusicApp::loadAllSongsToSongsMap(const int& id) noexcept {
    for (const auto& song : songsMap) {
        playlist->addMedia(QUrl::fromLocalFile(song.second->getQStrPath())); // performance is weak
    }

    if (ui->listOfSongs->count() == 0) {
        for (const auto& song : songsMap) ui->listOfSongs->addItem(song.second->show());
    }

    player->playlist()->setCurrentIndex(id);
}

void MusicApp::onItemDBClicked(QListWidgetItem* item) noexcept {
    player->playlist()->clear();
    std::cout << ui->listOfSongs->row(item) << std::endl;
    loadAllSongsToSongsMap(ui->listOfSongs->row(item));

    player->play();

    ui->playAndPause->setIcon(QPixmap(":/recources/images/pause.png"));
}

void MusicApp::onRightClick(QPoint point) noexcept {
    mainMenu->popup(ui->listOfSongs->viewport()->mapToGlobal(point));
}


void MusicApp::addSongToPlaylist(QString song, QString playlist) noexcept {
    std::string playlistName = playlist.toStdString();

    dbPtr->addSongToPlaylist(song, playlist);
    auto playlistOccurancy = playlistsMap.find(playlistName);
    playlistOccurancy->second.emplace_back(song.toStdString());
    // set global checker for plalist
    if (activePlaylist == playlistName) {
        reloadListOfPlaylistSongsView(playlistName);
    }
}

void MusicApp::onAddToClicked(QString text) noexcept {
    QString song = ui->listOfSongs->selectedItems().first()->text();
    addSongToPlaylist(song, text);
    // reload playlist
    reloadListOfPlaylistSongsView(text.toStdString());
}

void MusicApp::onPlayPauseClick() noexcept {
    isPlaying = !isPlaying;
    if (isPlaying) {
        player->play();
    } else {
        player->pause();
    }

    QPixmap icon = isPlaying ? QPixmap(":/recources/images/pause.png") : QPixmap(":/recources/images/play.png");
    ui->playAndPause->setIcon(icon);
}

void MusicApp::onNextSongClick() noexcept {
    player->playlist()->next();
    changeTitle();
}

void MusicApp::onPrevSongClick() noexcept {
    player->playlist()->previous();
    changeTitle();
}

void MusicApp::onLoopBtnClick() noexcept {
    auto loopMode = loop->nextLoopMode();
    player->playlist()->setPlaybackMode(loopMode.first);
    ui->loopBtn->setIcon(loopMode.second);
}

void MusicApp::onShuffleBtnClick() noexcept {
    auto shuffleMode = shuffle->nextShuffleMode();
    player->playlist()->setPlaybackMode(shuffleMode.first);
    ui->shuffleBtn->setIcon(shuffleMode.second);
}

void MusicApp::onCurrentMediaChanged(int id) noexcept {
    ui->listOfSongs->item(currentPlayingId)->setSelected(false);
    currentPlayingId = id;
    ui->listOfSongs->item(currentPlayingId)->setSelected(true);
}

void MusicApp::reloadListOfSongsView() noexcept {
    ui->listOfSongs->clear();
    for (auto song : songsMap) {
        ui->listOfSongs->addItem(song.second->show());
    }
}

void MusicApp::reloadListOfPlaylistSongsView(const std::string &playlist) noexcept {
    ui->listOfSongsInPlaylist->clear();
    auto occurancy = playlistsMap.find(playlist);
    for (auto song : occurancy->second) {
        ui->listOfSongsInPlaylist->addItem(QString(song.c_str()));
    }
}

void MusicApp::addPlaylistsToMenuActions(const std::string& oneItem) noexcept {
    QList<QAction*> listOfActions = oneItem.empty() ? MenuBuilder::addActionsToMenu(listOfPlaylistMenu, playlistsMap) : MenuBuilder::addActionsToMenu(listOfPlaylistMenu, oneItem);
    for (QAction* playlistAction : listOfActions) {
        QSignalMapper* mapper = new QSignalMapper(this);
        connect(playlistAction, SIGNAL(triggered(bool)), mapper, SLOT(map()));
        mapper->setMapping(playlistAction, playlistAction->text());
        connect(mapper, SIGNAL(mapped(QString)), this, SLOT(onAddToClicked(QString)));
    }
}

void MusicApp::on_addMusic_triggered() noexcept {
    QStringList songAbsPathes = fs::chooseSongsToAdd(this);

    if (!songAbsPathes.isEmpty()) {
        std::vector<std::string> songsPathes;
        for (QString& path : songAbsPathes) {
            songsPathes.emplace_back(path.toStdString());
        }

        auto newSongsMap = Song::getSongsMap(songsPathes);

        for (const auto& song : newSongsMap) {
            player->playlist()->addMedia(QUrl::fromLocalFile(song.second->getQStrPath()));
        }

        songsMap.insert(std::make_move_iterator(newSongsMap.begin()), std::make_move_iterator(newSongsMap.end()));
        dbPtr->addSongsPathes(songsPathes);

        reloadListOfSongsView(); // bad performance

    }
}

void MusicApp::on_addPlaylist_triggered() noexcept {
    form = make_unique<CreatePlaylistForm>(ui->listOfSongs);
    connect(form.get(), SIGNAL(sendPlaylistToMain(std::pair<QString, std::vector<std::string> >)), this, SLOT(onSendPlaylist(std::pair<QString, std::vector<std::string> >)));
    form->show();
}

void MusicApp::onSendPlaylist(std::pair<QString, std::vector<std::string> > received) noexcept {
    ui->listOfPlaylists->addItem(received.first);

    std::pair<std::string, std::vector<std::string>> item {received.first.toStdString(), received.second};
    playlistsMap.insert(item);

    addPlaylistsToMenuActions(item.first);
}

void MusicApp::onPlaylistClicked(QListWidgetItem *item) noexcept {
    auto occurancy = playlistsMap.find(item->text().toStdString());
    ui->listOfSongsInPlaylist->clear();
    for (auto label : occurancy->second) {
        ui->listOfSongsInPlaylist->addItem(QString(label.c_str()));
    }
}

void MusicApp::onPlaylistDBClicked(QListWidgetItem *item) noexcept {
    std::string playlistName = item->text().toStdString();
    activePlaylist = playlistName;

    player->playlist()->clear();

    std::vector<std::string> songs = playlistsMap.find(playlistName)->second;
    for (auto& song : songs) {
        auto songData = songsMap.find(song)->second;
        playlist->addMedia(QUrl::fromLocalFile(songData->getQStrPath()));
    }
    player->play();
    ui->playAndPause->setIcon(QPixmap(":/recources/images/pause.png"));
}

MusicApp::~MusicApp() {}
