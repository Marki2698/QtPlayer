#include "fs.h"
#include <QObject>
#include <QStringList>
#include <QFileDialog>
#include <string>


QStringList fs::chooseSongsToAdd(MusicApp *parent) noexcept {
    QStringList songsAbsPathes = QFileDialog::getOpenFileNames(parent,
                                                               QObject::tr("Open files"),
                                                               "",
                                                               QObject::tr("MP3 Files (*.mp3)"));
    return songsAbsPathes;
}

void fs::createUTF8File(const std::string filename) {
    if (!QFileInfo::exists(QString(filename.c_str()))) {
        FILE* file = fopen(filename.c_str(), "a+, ccs=UTF-8");
        fclose(file);
    }
}

