#include "types.h"

#include "removesongsform.h"
#include "ui_removesongsform.h"
#include "utils.h"
#include "song.h"
#include "fs.h"
#include "db.h"


RemoveSongsForm::RemoveSongsForm(QListWidget* const songList, std::shared_ptr<DB> dbPtr, QWidget *parent) :
    QDialog(parent),
    ui(make_unique<Ui::RemoveSongsForm>()),
    db(dbPtr)
{
    ui->setupUi(this);

    QStringList list {};

    for (int i = 0; i < songList->count(); ++i) {
        auto item = songList->item(i);
        list.push_back(item->text());
    }

    ui->listOfSongs->addItems(list);

    connect(ui->cancelRemoveBtn, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(ui->removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveClicked()));
    connect(ui->resetSelection, SIGNAL(clicked()), this, SLOT(onResetClicked()));
    connect(ui->selectAll, SIGNAL(clicked()), this, SLOT(onSelectAllClicked()));

}

RemoveSongsForm::~RemoveSongsForm() {}

void RemoveSongsForm::onSelectAllClicked() noexcept {
    ui->listOfSongs->selectAll();
}

void RemoveSongsForm::onResetClicked() noexcept {
    ui->listOfSongs->clearSelection();
}

void RemoveSongsForm::onCancelClicked() noexcept {
    this->close();
}

void RemoveSongsForm::onRemoveClicked() noexcept {
    QList<QString> qListOfSongs;

    for (auto item : ui->listOfSongs->selectedItems()) {
        qListOfSongs.append(item->text());
    }

    db->removeSongs(qListOfSongs);

    emit notifyMainWindow();

    this->close();
}
