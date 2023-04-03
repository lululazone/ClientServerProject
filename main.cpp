
#include <QCoreApplication>
#include "client.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Créer un serveur et le démarrer sur le port 1234
    Server server;
    server.startServer(1234);

    // Créer un client et se connecter au serveur
    Client client;
    client.connectToServer("localhost", 1234);

    // Envoyer un message du client au serveur
    client.sendMessage("Hello, server!");



    // Afficher les messages reçus par le serveur
    QObject::connect(&server, &Server::messageReceived, [](const QString &message) {
        qDebug() << "Server received message:" << message;
    });

    client.sendMessage("eheheh");

    QObject::connect(&server, &Server::messageReceived, [](const QString &message) {
        qDebug() << "Server received message:" << message;
    });

    // Envoyer un message du serveur à tous les clients
    server.sendMessage("Hello, clients!");




    return a.exec();

}
