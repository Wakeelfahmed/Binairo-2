#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // w.show();    //show the window when from the select_start_method
    return a.exec();
}
