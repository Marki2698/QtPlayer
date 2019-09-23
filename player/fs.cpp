#include "fs.h"
#include <QObject>
#include <QStringList>
#include <QFileDialog>

QStringList fs::chooseSongsToAdd(MusicApp *parent) noexcept {
    QStringList songsAbsPathes = QFileDialog::getOpenFileNames(parent,
                                                               QObject::tr("Open files"),
                                                               "",
                                                               QObject::tr("MP3 Files (*.mp3)"));
    return songsAbsPathes;
}

