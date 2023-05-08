#include "server.h"
#include "Lexer.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <string>
#include <QtConcurrent/QtConcurrent>
#include <omp.h>

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

void Server::onAsyncOperationFinished()
{
    isFinished = true;
    isStarted = false;
    loop.quit();
}

void Server::onAsyncOperationPause()
{
    isPaused = true;
    dbManager.db.close();

}

void Server::onAsyncOperationSuspend()
{
    dbManager.db.close();
}

void Server::onAsyncOperationResumed()
{
    dbManager.db.open();
}

void Server::scanDisk()
{
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(threaded()));
    thread.start();
    thread.setPriority(QThread::IdlePriority);
}

int Server::countFiles(const QString &path)
{
    QDir directory(path);

    // Compter les fichiers du répertoire actuel
    QFileInfoList files = directory.entryInfoList(QDir::Files);
    int fileCount = files.count();

    // Compter les fichiers des sous-répertoires en parallèle
    QFileInfoList dirs = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    #pragma omp parallel for reduction(+:fileCount)
    for (int i = 0; i < dirs.count(); ++i) {
        QString subdirPath = dirs.at(i).absoluteFilePath();
        fileCount += countFiles(subdirPath);
    }

    return fileCount;
}

void Server::threaded()
{
    isFinished = false;
    isPaused = false;
    isStopped = false;
    if(dbManager.db.open()){
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

    }
    QStringList pathList = dbManager.getTable("WHITELIST");
    int percentageMax = 1;
    int actualPercentage = 0;
    try{
        foreach (QString path, pathList) {
            emit sendMessage("Wait, preparing indexing...");
            percentageMax += countFiles(path);
            emit sendMessage("\n Counting files:"+percentageMax);
            qDebug() << percentageMax;
        }
    }
    catch(const std::exception){
        emit sendMessage("Error while trying to read path, have you provided valid path ?");
        return;
    }

    QStringList fileAlreadyInTable;
    QStringList blackList;
    QStringList filterList;
    QStringList skipped_filterList;
    QSqlQuery querySelect;
    querySelect.exec("SELECT * FROM files");
    int id = querySelect.record().indexOf("filename");
    while(querySelect.next()){
        fileAlreadyInTable.append(querySelect.value("filename").toString()+querySelect.value("ext").toString());
    }
    querySelect.exec("SELECT * FROM BLACKLIST");
    while(querySelect.next()){
        blackList.append(querySelect.value("name").toString());
    }
    querySelect.exec("SELECT * FROM FILTERS");
    while(querySelect.next()){
        filterList.append(querySelect.value("name").toString());
    }
    querySelect.exec("SELECT * FROM SKIPPED_FILTERS");
    while(querySelect.next()){
        skipped_filterList.append(querySelect.value("name").toString());
    }
    for(int i =0;i<pathList.size();i++){
        if(blackList.contains(pathList[i])){
            pathList.removeAt(i);
        }
    }
    try{
        for(int i =0; i<pathList.size();i++) {

            QDirIterator it(pathList[i], QDirIterator::Subdirectories);

            QString message = "";

            while (it.hasNext()) {

                message = it.next();

                QDir directory(message);
                QStringList files = directory.entryList(QStringList() << "*.*",QDir::Files);
                if(blackList.contains(message)){
                    files.clear();
                }
                #pragma omp for
                for(int j = 0; j<files.size();j++){
                    readData();
                    emit sendMessage("Trying to add file: "+files[j] + "\n ");
                    actualPercentage++;
                    int fixedpercentage = (actualPercentage*90)/percentageMax;
                    emit sendMessage(" percentage " + QString::number(fixedpercentage)+" ");
                    if(isPaused){
                        emit sendMessage("INDEXING PAUSED");
                    }

                    while(isPaused){
                        readData();
                    }
                    if(isStopped){
                        emit sendMessage("INDEXING STOPPED");
                        future.suspend();
                        return;
                    }

                    QFileInfo fileInfo(message+"/"+files[j]);
                    QDateTime creationDate = fileInfo.birthTime();
                    QDateTime lastModified = fileInfo.lastModified();
                    qint64 fileSize = fileInfo.size();
                    QString outputDebug = creationDate.toString() + lastModified.toString();


                    //getting the extension
                    QString extension;
                    int dot_pos = files[j].lastIndexOf(".");
                    if (dot_pos != -1) {
                        extension = files[j].mid(dot_pos + 1);
                    }

                    //getting the type
                    QString longType;
                    longType = QMimeDatabase().mimeTypeForFile(fileInfo.filePath()).name();
                    QStringList longTypeParts = longType.split("/");
                    QString shortType = longTypeParts.first();

                    //Add folder PATH to database (message) here...
                    QString path;
                    path = message + files[j];
                    QStringList fileName;
                    fileName = files[j].split(".");
                    files[j] = fileName[0];





                    //add file info to database here
                    QSqlQuery query;
                    if((filterList.empty() || filterList.contains(extension)) && !skipped_filterList.contains(extension) ){
                        if(!fileAlreadyInTable.contains(files[j]+extension)){
                            fileAlreadyInTable.append(path);
                            query.prepare("INSERT INTO files (filename, last_modified, creation_date, size, ext, type, path) VALUES (:filename, :last_modified, :creation_date, :size, :ext, :type, :path)");
                            query.bindValue(":filename", files[j]);
                            query.bindValue(":last_modified", lastModified.toString("yyyy-MM-dd"));
                            query.bindValue(":creation_date", creationDate.toString("yyyy-MM-dd"));
                            query.bindValue(":size", fileSize);
                            query.bindValue(":ext", extension);
                            query.bindValue(":type", shortType);
                            query.bindValue(":path", path);
                            bool isExec = query.exec();
                            if (!isExec) {
                                qDebug() << "Failed to add file to database: " << query.lastError().text();
                                qDebug() <<files[j]<<lastModified.toString("yyyy-MM-dd")<<creationDate.toString("yyyy-MM-dd")<<fileSize<<extension<<shortType<<path;
                                isExec = query.exec();
                            }


                        }
                        else{
                            emit sendMessage("Skipping "+files[j]+" because it's already in database\n ");
                        }

                    }
                    else{

                        actualPercentage++;
                        emit sendMessage("Skipping "+files[j]+" because it does not match filters\n ");
                        int fixedpercentage = (actualPercentage*100)/percentageMax;
                        emit sendMessage("\n percentage " + QString::number(fixedpercentage)+" ");
                    }

                }
            }

        }
    }
    catch(const std::exception){
        emit sendMessage("Error while trying to read path, have you provided valid path ?");
        return;
    }

    isFinished = true;
    isStarted = false;
    emit sendMessage("\n percentage 100 ");
    emit sendMessage("\n percentage 100 ");
    emit sendMessage("\n percentage 100 ");


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
        QString output;
        ErrorManager error = *new ErrorManager();
        if(message != ""){
            future.suspend();
            output = lexer.Tokenize(message,error,dbManager);
        }
        if(output == "start"){
            emit sendMessage("Indexing Started ");

            future = QtConcurrent::run(&Server::threaded,this);
            QFutureWatcher<void> watcher;

            connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()),Qt::QueuedConnection);
            connect(&watcher, SIGNAL(paused()), &loop, SLOT(onAsyncOperationPause()),Qt::QueuedConnection);
            connect(&watcher, SIGNAL(resumed()), &loop, SLOT(onAsyncOperationResumed()),Qt::QueuedConnection);
            connect(&watcher, SIGNAL(suspended()), &loop, SLOT(onAsyncOperationSuspend()),Qt::QueuedConnection);
            watcher.setFuture(future);
            loop.exec();
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
        if(future.isFinished()){
            emit sendMessage("Status: READY");
            return;
        }
        if(isPaused){
            emit sendMessage("Status: PAUSED");
            return;
        }
        if(future.isRunning()){
            emit sendMessage("Status: INDEXING");
            return;
        }
        if(future.isCanceled()){
            emit sendMessage("Status: STOPPED");
            return;
        }
}







void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}


