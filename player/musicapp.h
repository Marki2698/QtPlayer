#ifndef MUSICAPP_H
#define MUSICAPP_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QListWidgetItem>
#include <QString>
#include "db.h"
#include "song.h"
#include "loop.h"

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
    void onItemDBClicked(QListWidgetItem *item) noexcept;
    void onPlayPauseClick() noexcept;
    void onNextSongClick() noexcept;
    void onPrevSongClick() noexcept;
    void onLoopBtnClick() noexcept;

private:
    bool isPlaying = false;
    Ui::MusicApp *ui;
    QMediaPlayer* player;
    Loop* loop; // possibly make dynamically allocated
    std::unique_ptr<DB> dbPtr;
    std::unordered_map<std::string, std::unique_ptr<Song>> songsMap;
    void changeTitle() noexcept;

};

#endif // MUSICAPP_H
