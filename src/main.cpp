#include "RIM.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RIM w;



    w.show();
    return a.exec();
}
