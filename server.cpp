#include "server.h"
#include "Lexer.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <string>
#include <QtConcurrent/QtConcurrent>

using namespace std;

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(sendMessage(QString)), this, SLOT(sendMessageToClients(QString)));
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



void Server::sendMessageToClients(QString message)
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

void Server::scanDisk()
{
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(threaded()));
    thread.start();
    thread.setPriority(QThread::IdlePriority);
}

void Server::threaded()
{
    isFinished = false;
    isPaused = false;
    isStopped = false;
    if (dbManager.db.open()) {
        QSqlDatabase db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS 'files' ( \
                   id INTEGER PRIMARY KEY AUTOINCREMENT, \
                   filename TEXT NOT NULL, \
                   last_modified DATETIME NOT NULL, \
                   creation_date DATETIME NOT NULL, \
                   size INTEGER NOT NULL, \
                   ext TEXT NULL, \
                   type TEXT NULL, \
                   path TEXT NULL)");
            query.exec("INSERT INTO files (filename, last_modified, creation_date, max_size, min_size, size, ext, type, path) VALUES ('example.txt', '2022-03-15 13:45:00', '2022-01-01 09:00:00', 1000, 100, 500, 'txt', 'text/plain', 'something')");
    } else {
        QString error = dbManager.db.lastError().text();
    }
    QStringList pathList = dbManager.getTable("WHITELIST");
    int percentageMax = 0;
    int actualPercentage = 0;
    foreach (QString path, pathList) {
        percentageMax += QDir(path).entryList(QDir::AllEntries | QDir::NoDotAndDotDot).count();
        qDebug() << percentageMax;
    }
    foreach (QString path, pathList) {
        QDirIterator it(path, QDirIterator::Subdirectories);
        actualPercentage++;
        int fixedpercentage = (actualPercentage*100)/percentageMax;
        emit sendMessage("percentage " + QString::number(fixedpercentage)+" ");

        QString message = "";
        while (it.hasNext()) {
            if(isStopped){
                return;
            }
            while(isPaused){
                QThread::sleep(1);
            }

            message = it.next();

            QDir directory(message);
            QStringList files = directory.entryList(QStringList() << "*.*",QDir::Files);

            foreach(QString file, files){
                emit sendMessage("Adding file: "+file + "\n");
                QFileInfo fileInfo(message+"/"+file);
                QDateTime creationDate = fileInfo.birthTime();
                QDateTime lastModified = fileInfo.lastModified();
                qint64 fileSize = fileInfo.size();
                QString outputDebug = creationDate.toString() + lastModified.toString();

                //getting the extension
                QString extension;
                int dot_pos = file.lastIndexOf(".");
                if (dot_pos != -1) {
                    extension = file.mid(dot_pos + 1);
                }

                //getting the type
                QString longType;
                longType = QMimeDatabase().mimeTypeForFile(fileInfo.filePath()).name();
                QStringList longTypeParts = longType.split("/");
                QString shortType = longTypeParts.first();

                //Add folder PATH to database (message) here...
                QString path;
                path = message + file;

                //add file info to database here
                QSqlQuery query;
                query.prepare("INSERT INTO files (filename, last_modified, creation_date, size, ext, type, path) VALUES (:filename, :last_modified, :creation_date, :size, :ext, :type, :path)");
                query.bindValue(":filename", file);
                query.bindValue(":last_modified", lastModified.toString("yyyy-MM-dd hh:mm:ss"));
                query.bindValue(":creation_date", creationDate.toString("yyyy-MM-dd hh:mm:ss"));
                query.bindValue(":size", fileSize);
                query.bindValue(":ext", extension);
                query.bindValue(":type", shortType);
                query.bindValue(":path", path);
                if (!query.exec()) {
                    qDebug() << "Failed to add file to database: " << query.lastError().text();
                }
            }
        }

    }
    isFinished = true;
    isStarted = false;

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
        if(output == "start"){
            emit sendMessage("Indexing Started ");
            QFuture<void> future = QtConcurrent::run(&Server::threaded,this);
            isStarted = true;
            return;
        }
        if(output == "stop"){
            isStopped = true;
            return;
        }
        if(output == "pause"){
            isPaused = true;
            return;
        }
        if(output == "resume"){
            isPaused = false;
            return;
        }
        if(output == "statut"){
            getStatut();
            return;
        }
        emit sendMessage(output);
}

void Server::getStatut()
{
        if(isFinished && !isStarted){
            emit sendMessage("Status: READY");
            return;
        }
        if(isPaused && isStarted){
            emit sendMessage("Status: PAUSED");
            return;
        }
        if(!isFinished && isStarted){
            emit sendMessage("Status: INDEXING");
            return;
        }
        if(isStopped && isStarted){
            emit sendMessage("Status: STOPPED");
            return;
        }
        emit sendMessage("Status: RESULTS_AVAILABLE");
}







void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


