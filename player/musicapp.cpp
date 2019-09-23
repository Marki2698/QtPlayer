#include "musicapp.h"
#include "ui_musicapp.h"
#include "fs.h"
#include "song.h"

#include <iostream>
#include <QString>
#include <QStringList>
#include <QMediaPlayer>
#include <QFile>

MusicApp::MusicApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicApp)
{
    ui->setupUi(this);
}

void MusicApp::on_addMusic_triggered() noexcept{
    QStringList songAbsPathes = fs::chooseSongsToAdd(this);
    for (QString path : songAbsPathes) {
        // call db;
        // create song instance

//        Song* song = new Song(path);
//        std::cout << path.toStdString() << std::endl;
//        std::cout << (*song) << std::endl;
//        delete song;

        player = new QMediaPlayer();
        QObject::connect(player,
                         SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
                         this,
                         SLOT(onRetrieveSongData(QMediaPlayer::MediaStatus)));

        player->setMedia(QUrl::fromLocalFile(path));
    }
}

void MusicApp::onRetrieveSongData(QMediaPlayer::MediaStatus status) noexcept {
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
        std::cout << "here" << std::endl;
        std::cout << player->media().resources().first().url().path().toStdString() << std::endl;
        player->play();
    }
}

MusicApp::~MusicApp()
{
    player->stop();
    delete player;
    delete ui;
}
