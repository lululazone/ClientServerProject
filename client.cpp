#include "client.h"
#include "mainwindow.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Client::connectToServer(QString host, quint16 port)
{
    socket->connectToHost(host, port);
}

Client::~Client()
{
    delete socket;
}


void Client::sendMessage(QString message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << quint16(0); // convertir la chaîne de caractères en UTF-8

    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    socket->write(message.toUtf8());
}

void Client::readData()
{
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data); // convertir les données en UTF-8
    emit messageReceived(message);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Client error:" << socketError;
}



