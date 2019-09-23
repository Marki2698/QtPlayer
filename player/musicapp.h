#ifndef MUSICAPP_H
#define MUSICAPP_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QString>

namespace Ui {
class MusicApp;
}

class MusicApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicApp(QWidget *parent = 0);
    ~MusicApp();

public slots:
    void on_addMusic_triggered() noexcept;
    void onRetrieveSongData(QMediaPlayer::MediaStatus status) noexcept;

private:
    Ui::MusicApp *ui;
    QMediaPlayer* player;
};

#endif // MUSICAPP_H
