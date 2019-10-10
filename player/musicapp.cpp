#include "musicapp.h"
#include "ui_musicapp.h"
#include "fs.h"
#include "song.h"
#include "db.h"
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
    player(new QMediaPlayer)
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

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onRetrieveSongData(QMediaPlayer::MediaStatus)));
}

void MusicApp::onPlayPauseClick() noexcept {
    isPlaying = !isPlaying;
    if (isPlaying) {
        player->play();
    } else {
        player->pause();
    }
}

void MusicApp::onNextSongClick() noexcept {
    player->playlist()->next();
    changeTitle();
}

void MusicApp::onPrevSongClick() noexcept {
    player->playlist()->previous();
    changeTitle();
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

//    player = new QMediaPlayer();
//    QObject::connect(player,
//                     SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
//                     this,
//                     SLOT(onRetrieveSongData(QMediaPlayer::MediaStatus))
//                     );

//    player->setPlaylist(playlist);
}

void MusicApp::onRetrieveSongs(QMediaPlayer::MediaStatus status) noexcept {
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
        std::cout << "here" << std::endl;
        // isAssert error .first() method
//        while (player->playlist()->currentIndex() < player->playlist()->mediaCount() ) {
//            for (QMediaResource resource : player->playlist()->currentMedia().resources()) {
//                std::cout << resource.url().path().toStdString() << std::endl;
//            }

//            player->playlist()->setCurrentIndex(player->playlist()->nextIndex());
//        }


//        for (QMediaResource resource : player->playlist()->currentMedia().resources()) {
//            std::cout << resource.url().path().toStdString() << std::endl;
//        }

//        std::cout << player->media().resources().size() << std::endl;
//        for (QMediaContent content : player->media().resources()) {
//            std::cout << content.resources().first().url().path().toStdString() << std::endl;
//        }
//        delete player;

//        for (QMediaResource resource : player->media().resources()) {
//            std::cout << resource.url().path().toStdString() << std::endl;
//        }

//        QStringListModel* model = new QStringListModel();
//        QStringList list;
//        list << player->metaData(QMediaMetaData::Title).toStringList();
//        model->setStringList(list);
//        ui->listOfSongs->setModel(model);
//        player->play();
    }
}

void MusicApp::onRetrieveSongData(QMediaPlayer::MediaStatus status) noexcept{
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
//        player->metaData(QMediaMetaData::)
        player->play();
        changeTitle();
        isPlaying = true;
    }
}

MusicApp::~MusicApp()
{
//    player->stop();
    delete player;
    delete ui;
}
