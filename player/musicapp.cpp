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

#include <iostream>
#include <memory>
#include <utility>

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
#include <fstream>
#include <string>


MusicApp::MusicApp(QWidget *parent) :
    QMainWindow(parent),
    ui(make_unique<Ui::MusicApp>()),
    player(make_unique<QMediaPlayer>()),
    loop(make_unique<Loop>()),
    shuffle(make_unique<Shuffle>()),
    dbPtr(make_unique<DB>(STUB_FILE_PATH, PLAYLIST_DIR_PATH))
{
    fs::createUTF8File(STUB_FILE_PATH);
    fs::createPlaylistFolder(PLAYLIST_DIR_PATH);

    ui->setupUi(this);
    ui->listOfSongs->setSelectionMode(QAbstractItemView::ExtendedSelection);

    songsMap = Song::getSongsMap(dbPtr->getSongsPathes());
    playlistsMap = dbPtr->getPlaylistsSongsPathes();

    for (const auto& playlist : playlistsMap) {
        ui->listOfPlaylists->addItem(QString(playlist.first.c_str()));
    }

    playlist = make_unique<QMediaPlaylist>();

    player->setPlaylist(playlist.get());
    loadAllSongsToSongsMap(player->playlist()->currentIndex());

    form = make_unique<CreatePlaylistForm>(ui->listOfSongs);


    connect(ui->listOfSongs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDBClicked(QListWidgetItem*)));
    connect(ui->playAndPause, SIGNAL(clicked()), this, SLOT(onPlayPauseClick()));
    connect(ui->nextSong, SIGNAL(clicked()), this, SLOT(onNextSongClick()));
    connect(ui->prevSong, SIGNAL(clicked()), this, SLOT(onPrevSongClick()));
    connect(ui->loopBtn, SIGNAL(clicked()), this, SLOT(onLoopBtnClick()));
    connect(ui->shuffleBtn, SIGNAL(clicked()), this, SLOT(onShuffleBtnClick()));
    connect(playlist.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentMediaChanged(int)));
    connect(ui->listOfPlaylists, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onPlaylistClicked(QListWidgetItem*)));
    connect(ui->listOfPlaylists, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onPlaylistDBClicked(QListWidgetItem*)));
    connect(form.get(), SIGNAL(sendPlaylistToMain(std::pair<QString, std::vector<std::string> >)), this, SLOT(onSendPlaylist(std::pair<QString, std::vector<std::string> >)));
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
    form->show();
}

void MusicApp::onSendPlaylist(std::pair<QString, std::vector<std::string> > received) noexcept {
    ui->listOfPlaylists->addItem(received.first);
    playlistsMap.insert({received.first.toStdString(), received.second});
//    std::copy(received.second.cbegin(), received.second.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

void MusicApp::onPlaylistClicked(QListWidgetItem *item) noexcept {
    auto occurancy = playlistsMap.find(item->text().toStdString());
    ui->listOfSongsInPlaylist->clear();
    for (auto label : occurancy->second) {
        ui->listOfSongsInPlaylist->addItem(QString(label.c_str()));
    }
}

void MusicApp::onPlaylistDBClicked(QListWidgetItem *item) noexcept {
    player->playlist()->clear();

    std::vector<std::string> songs = playlistsMap.find(item->text().toStdString())->second;
    for (auto& song : songs) {
        auto songData = songsMap.find(song)->second;
        playlist->addMedia(QUrl::fromLocalFile(songData->getQStrPath()));
    }
    player->play();
    ui->playAndPause->setIcon(QPixmap(":/recources/images/pause.png"));
}

MusicApp::~MusicApp() {}
