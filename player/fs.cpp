#include "fs.h"
#include <QObject>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <string>
#include <iostream>


QStringList fs::chooseSongsToAdd(MusicApp *parent) noexcept {
    QStringList songsAbsPathes = QFileDialog::getOpenFileNames(parent,
                                                               QObject::tr("Open files"),
                                                               "",
                                                               QObject::tr("MP3 Files (*.mp3)"));
    return songsAbsPathes;
}

void fs::createUTF8File(const std::string& filename) {
    if (!QFileInfo::exists(QString(filename.c_str()))) {
        FILE* file = fopen(filename.c_str(), "a+, ccs=UTF-8");
        fclose(file);
    }
}

void fs::createPlaylistFolder(const std::string &dirname) {
    std::string path = "./" + dirname;
    QString Qpath(path.c_str());
//    QString Qdirname(dirname.c_str());

    QDir dir(Qpath);
    if (!dir.exists()) {
        dir.mkpath(Qpath);
    } else {
        std::cout << "already exists" << std::endl;
    }
}

