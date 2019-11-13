#ifndef REMOVESONGSFORM_H
#define REMOVESONGSFORM_H


#include "types.h"

#include "song.h"
#include "db.h"

namespace Ui {
class RemoveSongsForm;
}

class RemoveSongsForm : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveSongsForm(QListWidget* songList, std::shared_ptr<DB> dbPtr, QWidget *parent = 0);
    ~RemoveSongsForm();

public slots:
    void onSelectAllClicked() noexcept;
    void onResetClicked() noexcept;
    void onCancelClicked() noexcept;
    void onRemoveClicked() noexcept;

signals:
    void notifyMainWindow();

private:
    std::unique_ptr<Ui::RemoveSongsForm> ui;
    std::shared_ptr<DB> db;
};

#endif // REMOVESONGSFORM_H
