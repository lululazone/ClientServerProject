#include "server.h"
#include "Lexer.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>

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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
       db.setDatabaseName("./qtdb.db");
    if (db.open()) {
         QSqlDatabase db = QSqlDatabase::database("QSQLITE");
         QSqlQuery query;
         query.exec("CREATE TABLE IF NOT EXISTS 'files' ( \
         id INTEGER PRIMARY KEY AUTOINCREMENT, \
         filename TEXT NOT NULL, \
         last_modified DATETIME NOT NULL, \
         creation_date DATETIME NOT NULL, \
         size INTEGER NOT NULL, \
         ext TEXT NULL, \
         type TEXT NULL)");
        query.exec("INSERT INTO files (filename, last_modified, creation_date, max_size, min_size, size, ext, type) VALUES ('example.txt', '2022-03-15 13:45:00', '2022-01-01 09:00:00', 1000, 100, 500, 'txt', 'text/plain')");
    } else {
        QString error = db.lastError().text();
    }

        QDirIterator it("C:/fakefolder", QDirIterator::Subdirectories);
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
                QSqlQuery query;
                           query.prepare("INSERT INTO files (filename, last_modified, creation_date, size) VALUES (:filename, :last_modified, :creation_date, :size)");
                           query.bindValue(":filename", file);
                           query.bindValue(":last_modified", lastModified.toString("yyyy-MM-dd hh:mm:ss"));
                           query.bindValue(":creation_date", creationDate.toString("yyyy-MM-dd hh:mm:ss"));
                           query.bindValue(":size", fileSize);
//                           query.bindValue(":ext", fileInfo.suffix());
//                           query.bindValue(":type", QMimeDatabase().mimeTypeForFile(fileInfo.filePath()).name());
                           if (!query.exec()) {
                               qDebug() << "Failed to add file to database: " << query.lastError().text();
                           }
            }
        }
        sendMessage("Done !");

}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


