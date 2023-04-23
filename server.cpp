#include "server.h"
#include "Lexer.h"

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
        if(message == "Database init"){
            scanDisk();
            return;
        }
        qDebug() <<message;
        Lexer lexer;
        ErrorManager error = *new ErrorManager();
        QString output = lexer.Tokenize(message,error);
        sendMessage(output);
}

void Server::scanDisk()
{
        moveToThread(&thread);
        connect(&thread, SIGNAL(started()), this, SLOT(threaded()));
        thread.start();
        thread.setPriority(QThread::LowPriority);

}


void Server::threaded(){
        QDirIterator it("C:/", QDirIterator::Subdirectories);
        sendMessage("Processing....");
        QString message = "";
        while (it.hasNext()) {
            message = it.next();
            //Add folder to database (message) here....
            QDir directory(message);
            QStringList files = directory.entryList(QStringList() << "*.*",QDir::Files);
            foreach(QString file, files){
                // add filename to database here...
                qDebug()<<file;
                QFileInfo fileInfo(message+"/"+file);

                QDateTime creationDate = fileInfo.birthTime();
                QDateTime lastModified = fileInfo.lastModified();
                qint64 fileSize = fileInfo.size();
                QString outputDebug = creationDate.toString() + lastModified.toString();
                qDebug() << outputDebug;
                //add file info to database here...
            }

        }
        sendMessage("Done !");

}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


