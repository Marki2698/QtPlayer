#ifndef CREATEPLAYLISTFORM_H
#define CREATEPLAYLISTFORM_H

#include "types.h"

#include "song.h"
#include "db.h"


namespace Ui {
class CreatePlaylistForm;
}

class CreatePlaylistForm : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlaylistForm(QListWidget* songList, std::shared_ptr<DB> dbPtr, QWidget *parent = 0);
    ~CreatePlaylistForm();

public slots:
    void onSelectAllClicked() noexcept;
    void onResetClicked() noexcept;
    void onCancelClicked() noexcept;
    void onCreateClicked() noexcept;

signals:
    void sendPlaylistToMain(std::pair<QString, songsVectorT>);

private:
    std::unique_ptr<Ui::CreatePlaylistForm> ui;
    std::shared_ptr<DB> db;
};

#endif // CREATEPLAYLISTFORM_H
