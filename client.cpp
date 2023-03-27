#include "client.h"

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

    out << quint16(0);
    out << message;

    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    socket->write(data);
}

void Client::readData()
{
    QDataStream in(socket);

    if (socket->bytesAvailable() < sizeof(quint16))
        return;

    quint16 messageSize;
    in >> messageSize;

    if (socket->bytesAvailable() < messageSize)
        return;

    QString message;
    in >> message;

    emit messageReceived(message);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Client error:" << socketError;
}



