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
    void onRetrieveSongs(QMediaPlayer::MediaStatus status) noexcept;
    void onRetrieveSongData(QMediaPlayer::MediaStatus status) noexcept;
    void onItemDBClicked(QListWidgetItem *item) noexcept;
    void onPlayPauseClick() noexcept;
    void onNextSongClick() noexcept;
    void onPrevSongClick() noexcept;

private:
    bool isPlaying;
    Ui::MusicApp *ui;
    QMediaPlayer* player;
    std::unique_ptr<DB> dbPtr;
//    std::vector<std::unique_ptr<Song>> songs;
    std::unordered_map<std::string, std::unique_ptr<Song>> songsMap;
    void changeTitle() noexcept;

};

#endif // MUSICAPP_H
