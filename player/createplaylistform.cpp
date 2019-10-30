#include <QListWidget>
#include <QStringList>
#include <QString>

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "createplaylistform.h"
#include "ui_createplaylistform.h"
#include "utils.h"
#include "song.h"
#include "fs.h"
#include "db.h"
#include "types.h"

CreatePlaylistForm::CreatePlaylistForm(QListWidget* const songList, QWidget *parent) :
    QDialog(parent),
    ui(make_unique<Ui::CreatePlaylistForm>())
{
    ui->setupUi(this);

    QStringList list {};

    for (int i = 0; i < songList->count(); ++i) {
        auto item = songList->item(i);
        list.push_back(item->text());
    }

    ui->listOfSongs->addItems(list);

    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(ui->createPlaylist, SIGNAL(clicked()), this, SLOT(onCreateClicked()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(onResetClicked()));
    connect(ui->selectAll, SIGNAL(clicked()), this, SLOT(onSelectAllClicked()));
}

CreatePlaylistForm::~CreatePlaylistForm() {}


void CreatePlaylistForm::onSelectAllClicked() noexcept {
    ui->listOfSongs->selectAll();
}

void CreatePlaylistForm::onResetClicked() noexcept {
    ui->listOfSongs->clearSelection();
}

void CreatePlaylistForm::onCancelClicked() noexcept {
    this->close();
}

void CreatePlaylistForm::onCreateClicked() noexcept {
    if (ui->nameOfPlaylist->text().size() != 0) {
        QString playlistName = ui->nameOfPlaylist->text();
        std::string filename = playlistName.toStdString();
        std::string playlistPath = PLAYLIST_DIR_PATH + "/" + filename;
        fs::createUTF8File(playlistPath);

        std::string defaultPath = "";
        std::unique_ptr<DB> db = make_unique<DB>(playlistPath, defaultPath);

        songsVectorT pathes{};


        if (!ui->listOfSongs->selectedItems().empty()) {
            for (auto item: ui->listOfSongs->selectedItems()) {
                pathes.emplace_back(item->text().toStdString());
            }
        }


        db->addSongsPathes(pathes);

        emit sendPlaylistToMain({playlistName, pathes});
        this->close();
    }

}
