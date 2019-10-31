#ifndef CREATEPLAYLISTFORM_H
#define CREATEPLAYLISTFORM_H

#include "types.h"

#include "song.h"


namespace Ui {
class CreatePlaylistForm;
}

class CreatePlaylistForm : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlaylistForm(QListWidget* songList, QWidget *parent = 0);
    ~CreatePlaylistForm();

public slots:
    void onSelectAllClicked() noexcept;
    void onResetClicked() noexcept;
    void onCancelClicked() noexcept;
    void onCreateClicked() noexcept;

signals:
    void sendPlaylistToMain(std::pair<QString, std::vector<std::string>>);

private:
    std::unique_ptr<Ui::CreatePlaylistForm> ui;
};

#endif // CREATEPLAYLISTFORM_H
