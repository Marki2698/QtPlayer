#include "musicapp.h"
#include "ui_musicapp.h"
#include "fs.h"
#include "song.h"
#include "db.h"
#include "loop.h"
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/audioproperties.h"

#include <iostream>
#include <memory>
#include <utility>

#include <QString>
#include <QStringList>
#include <QMediaPlayer>
#include <QFile>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QUrl>
#include <QPixmap>
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
    loop(new Loop)
{
    std::string name = "stub.txt";
    createUTF8File(name);

    ui->setupUi(this);
    ui->listOfSongs->setSelectionMode(QAbstractItemView::ExtendedSelection);
    dbPtr.reset(new DB(std::move(true)));

    connect(ui->listOfSongs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDBClicked(QListWidgetItem*)));
    connect(ui->playAndPause, SIGNAL(clicked()), this, SLOT(onPlayPauseClick()));
    connect(ui->nextSong, SIGNAL(clicked()), this, SLOT(onNextSongClick()));
    connect(ui->prevSong, SIGNAL(clicked()), this, SLOT(onPrevSongClick()));
    connect(ui->loopBtn, SIGNAL(clicked()), this, SLOT(onLoopBtnClick()));

    songsMap = Song::getSongsMap(dbPtr->getSongsPathes());

    QMediaPlaylist* playlist = new QMediaPlaylist; // maybe leak
    for (const auto& song : songsMap) {
        playlist->addMedia(QUrl::fromLocalFile(song.second->getQStrPath()));
        ui->listOfSongs->addItem(song.second->show());
    }
    player->setPlaylist(playlist);

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
//    player->stop();
    delete loop;
    delete player;
    delete ui;
}
