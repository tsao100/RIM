#include "RIM.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RIM w;
    w.show();
    return a.exec();
}
