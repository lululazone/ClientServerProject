#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Envoyer un message du serveur à tous les clients



    w.show();
    return a.exec();

}


