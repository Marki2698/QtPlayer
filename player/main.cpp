#include "musicapp.h"
#include <QApplication>
#include <locale>

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "Ukrainian");
    QApplication a(argc, argv);
    MusicApp w;
    w.show();

    return a.exec();
}
