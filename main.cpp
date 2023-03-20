#include <QCoreApplication>
#include "myserver.h"
#include "sockettest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyServer myserver;
    SocketTest socket;
    socket.Connect();
    myserver.newConnection();
    socket.SendMessage("Bonjour");



    return a.exec();
}
