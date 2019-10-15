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

#include <iostream>
#include <memory>
#include <utility>

#include <QString>
#include <QStringList>
#include <QList>
#include <QMediaPlayer>
#include <QFile>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QUrl>
#include <QPixmap>
#include <Qt>
#include <fstream>
#include <string>

void createUTF8File(const std::string filename) {
    if (!QFileInfo::exists(QString(filename.c_str()))) {
        FILE* file = fopen(filename.c_str(), "a+, ccs=UTF-8");
        fclose(file);
    }
}


MusicApp::MusicApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicApp),
    player(new QMediaPlayer),
    loop(new Loop),
    shuffle(new Shuffle)
{
    std::string name = "stub.txt";
    createUTF8File(name);

    ui->setupUi(this);
    ui->listOfSongs->setSelectionMode(QAbstractItemView::ExtendedSelection);
    dbPtr.reset(new DB(std::move(true)));

    songsMap = Song::getSongsMap(dbPtr->getSongsPathes());

    playlist = new QMediaPlaylist;

    for (const auto& song : songsMap) {
        playlist->addMedia(QUrl::fromLocalFile(song.second->getQStrPath()));
        ui->listOfSongs->addItem(song.second->show());
    }
    player->setPlaylist(playlist);


    connect(ui->listOfSongs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDBClicked(QListWidgetItem*)));
    connect(ui->playAndPause, SIGNAL(clicked()), this, SLOT(onPlayPauseClick()));
    connect(ui->nextSong, SIGNAL(clicked()), this, SLOT(onNextSongClick()));
    connect(ui->prevSong, SIGNAL(clicked()), this, SLOT(onPrevSongClick()));
    connect(ui->loopBtn, SIGNAL(clicked()), this, SLOT(onLoopBtnClick()));
    connect(ui->shuffleBtn, SIGNAL(clicked()), this, SLOT(onShuffleBtnClick()));
    connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentMediaChanged(int)));

}

void MusicApp::onItemDBClicked(QListWidgetItem* item) noexcept {
    int id = ui->listOfSongs->row(item);
    player->playlist()->setCurrentIndex(id);
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

void MusicApp::changeTitle() noexcept {
    ui->title->setText(player->metaData(QMediaMetaData::Title).toString());
}

void MusicApp::on_addMusic_triggered() noexcept {
    QStringList songAbsPathes = fs::chooseSongsToAdd(this);

    if (!songAbsPathes.isEmpty()) {
        std::vector<std::string> songsPathes;
        for (QString& path : songAbsPathes) {
            songsPathes.push_back(path.toStdString());
        }

        auto newSongsMap = Song::getSongsMap(songsPathes);

        for (const auto& song : newSongsMap) {
            ui->listOfSongs->addItem(song.second->show());
            player->playlist()->addMedia(QUrl::fromLocalFile(song.second->getQStrPath()));
        }

        songsMap.insert(std::make_move_iterator(newSongsMap.begin()), std::make_move_iterator(newSongsMap.end()));

        dbPtr->addSongsPathes(songsPathes);

    }
}

MusicApp::~MusicApp()
{
    delete loop;
    delete shuffle;
    delete player;
    delete ui;
}
