#include "types.h"

#include "createplaylistform.h"
#include "ui_createplaylistform.h"
#include "utils.h"
#include "song.h"
#include "fs.h"
#include "db.h"


CreatePlaylistForm::CreatePlaylistForm(QListWidget* const songList, std::shared_ptr<DB> dbPtr, QWidget *parent) :
    QDialog(parent),
    ui(make_unique<Ui::CreatePlaylistForm>()),
    db(dbPtr)
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

        songsVectorT songs{};


        if (!ui->listOfSongs->selectedItems().empty()) {
            for (auto item: ui->listOfSongs->selectedItems()) {
                songs.emplace_back(item->text().toStdString());
            }
        }


        db->addPlaylist(playlistName, songs);


        emit sendPlaylistToMain({playlistName, songs});
        this->close();
    }

}
