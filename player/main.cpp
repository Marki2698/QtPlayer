#include "musicapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicApp w;
    w.show();

    return a.exec();
}
