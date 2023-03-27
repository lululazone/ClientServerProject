#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Server::startServer(quint16 port)
{
    if (server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Unable to start server on port" << port;
    }
}

void Server::sendMessage(QString message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << quint16(0);
    out << message;

    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    for (QTcpSocket *client : clients) {
        client->write(data);
    }
}

void Server::newConnection()
{
    QTcpSocket *client = server->nextPendingConnection();

    clients.append(client);

    connect(client, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));

    qDebug() << "New client connected";
}


void Server::readData()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    if (!client)
        return;

    QDataStream in(client);

    if (client->bytesAvailable() < sizeof(quint16))
        return;

    quint16 messageSize;
    in >> messageSize;

    if (client->bytesAvailable() < messageSize)
        return;

    QString message;
    in >> message;

    emit messageReceived(message);
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


