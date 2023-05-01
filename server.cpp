#include "server.h"
#include "Lexer.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <string>

using namespace std;

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    dbManager.db.open();
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

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0);
    out << message.toUtf8();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    for (QTcpSocket *client : clients) {
        client->write(message.toUtf8());
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
        QByteArray data = client->readAll();
        QString message = QString::fromUtf8(data);
        qDebug() <<message;
        Lexer lexer;
        ErrorManager error = *new ErrorManager();
        QString output = lexer.Tokenize(message,error,dbManager);
        sendMessage(output);
}





void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


