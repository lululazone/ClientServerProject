
#include "threadedoperation.h"
#include "dbinteraction.h"
#include "qdiriterator.h"
#include "qmimedatabase.h"
#include "qsqlerror.h"
#include "qsqlquery.h"

ThreadedOperation::ThreadedOperation()
{

}

void ThreadedOperation::scanDisk(QString path, QSqlDatabase dbManager)
{
    db=dbManager;
    pathIn=path;
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(threaded()));
    thread.start();
    thread.setPriority(QThread::LowestPriority);
}

void ThreadedOperation::threaded()
{
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
                   type TEXT NULL, \
                   path TEXT NULL)");
            query.exec("INSERT INTO files (filename, last_modified, creation_date, max_size, min_size, size, ext, type, path) VALUES ('example.txt', '2022-03-15 13:45:00', '2022-01-01 09:00:00', 1000, 100, 500, 'txt', 'text/plain', 'something')");
    } else {
        QString error = db.lastError().text();
    }

    QDirIterator it(pathIn, QDirIterator::Subdirectories);
    QString message = "";
    while (it.hasNext()) {
        message = it.next();

        QDir directory(message);
        QStringList files = directory.entryList(QStringList() << "*.*",QDir::Files);
        foreach(QString file, files){
            qDebug()<<file;
            QFileInfo fileInfo(message+"/"+file);
            QDateTime creationDate = fileInfo.birthTime();
            QDateTime lastModified = fileInfo.lastModified();
            qint64 fileSize = fileInfo.size();
            QString outputDebug = creationDate.toString() + lastModified.toString();
            qDebug() << outputDebug;

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
            qDebug() << path;

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

