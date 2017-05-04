#include "principal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    principal w;
    w.showMaximized();


    return a.exec();
}
