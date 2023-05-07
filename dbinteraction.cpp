
#include "dbinteraction.h"
#include "qdiriterator.h"
#include "qmimedatabase.h"
#include "qobjectdefs.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qsqlrecord.h"
#include "threadedoperation.h"

DbInteraction::DbInteraction()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
        qDebug() << "Database open";
        initTables();
    }

    //scanDisk("WHITELIST");
}

QString DbInteraction::deleteTable(QString type)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
        if(type == "WHITELIST"){
            QSqlQuery query;
            query.prepare("DELETE FROM WHITELIST");
            if(!query.exec()){
                return "failed to delete table: "+type;
            }
            return "Successfully removed table: "+type;
        }
        if(type == "BLACKLIST"){
            QSqlQuery query;
            query.prepare("DELETE FROM BLACKLIST");
            if(!query.exec()){
                return "failed to delete table: "+type;
            }
            return "Successfully removed table: "+type;
        }
        if(type == "FILTERS"){
            QSqlQuery query;
            query.prepare("DELETE FROM FILTERS");
            if(!query.exec()){
                return "failed to delete table: "+type;
            }
            return "Successfully removed table: "+type;
        }
        if(type == "SKIPPED_FILTERS"){
            QSqlQuery query;
            query.prepare("DELETE FROM SKIPPED_FILTERS");
            if(!query.exec()){
                return "failed to delete table: "+type;
            }
            return "Successfully removed table: "+type;
        }
        if(type == "FILES"){
            QSqlQuery query;
            query.prepare("DELETE FROM files");
            if(!query.exec()){
                return "failed to delete table: "+type;
            }
            return "Successfully removed table: "+type;
        }

    }
    return "failed to connect to the database";
}

QStringList DbInteraction::getTable(QString type)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
        if(type == "WHITELIST"){
            QSqlQuery query;
            query.prepare("SELECT * FROM WHITELIST");
            query.exec();
            QStringList result;
            if(!query.exec()){
                return result;
            }
            int id = query.record().indexOf("name");
            while(query.next()){
                result.append(query.value(id).toString());
            }
            return result;
        }
        if(type == "BLACKLIST"){
            QSqlQuery query;
            query.prepare("SELECT * FROM BLACKLIST");
            query.exec();
            QStringList result;
            if(!query.exec()){
                return result;
            }
            int id = query.record().indexOf("name");
            while(query.next()){
                result.append(query.value(id).toString());
            }
            return result;
        }
        if(type == "FILTERS"){
            QSqlQuery query;
            query.prepare("SELECT * FROM FILTERS");
            query.exec();
            QStringList result;
            if(!query.exec()){
                return result;
            }
            int id = query.record().indexOf("name");
            while(query.next()){
                result.append(query.value(id).toString());
            }
            return result;
        }
        if(type == "SKIPPED_FILTERS"){
            QSqlQuery query;
            query.prepare("SELECT * FROM SKIPPED_FILTERS");
            query.exec();
            QStringList result;
            if(!query.exec()){
                return result;
            }
            int id = query.record().indexOf("name");
            while(query.next()){
                result.append(query.value(id).toString());
            }
            return result;
        }


    }
    return {"failed to connect to the database"};
}

void DbInteraction::scanDisk(QString type)
{
    ThreadedOperation threaded;
    QStringList pathList = getTable(type);
    if (pathList.isEmpty()){
        return;
    }
    foreach (QString path, pathList) {
            threaded.scanDisk(path, db);
    }

}

QString DbInteraction::appendToTable(QString tableName, QString value)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
        if(tableName=="WHITELIST"){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO WHITELIST (name) VALUES (:value)");
            insertQuery.bindValue(":value",value);
            if(!insertQuery.exec()){
                qDebug() << insertQuery.lastError();
            }
            return value+" added to "+tableName+" successfully";
        }
        if(tableName=="BLACKLIST"){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO BLACKLIST (name) VALUES (:value)");
            insertQuery.bindValue(":tableName",tableName);
            insertQuery.bindValue(":value",value);
            if(!insertQuery.exec()){
                qDebug() << insertQuery.lastError();
            }
            return value+" added to "+tableName+" successfully";
        }
        if(tableName=="FILTERS"){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO FILTERS (name) VALUES (:value)");
            insertQuery.bindValue(":tableName",tableName);
            insertQuery.bindValue(":value",value);
            if(!insertQuery.exec()){
                qDebug() << insertQuery.lastError();
            }
            return value+" added to "+tableName+" successfully";
        }
        if(tableName=="SKIPPED_FILTERS"){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO SKIPPED_FILTERS (name) VALUES (:value)");
            insertQuery.bindValue(":tableName",tableName);
            insertQuery.bindValue(":value",value);
            if(!insertQuery.exec()){
                qDebug() << insertQuery.lastError();
            }
            return value+" added to "+tableName+" successfully";
        }
    }
    else{
        return "Database error";
    }
    return "";

}

void DbInteraction::initTables()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
        QSqlDatabase db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query;
        query.prepare("CREATE TABLE IF NOT EXISTS 'WHITELIST' ( \
                      id INTEGER PRIMARY KEY AUTOINCREMENT, \
                      name TEXT NOT NULL)");
        query.exec();
        query.prepare("CREATE TABLE IF NOT EXISTS 'BLACKLIST' ( \
                      id INTEGER PRIMARY KEY AUTOINCREMENT, \
                      name TEXT NOT NULL)");
            query.exec();
        query.prepare("CREATE TABLE IF NOT EXISTS 'FILTERS'( \
                      id INTEGER PRIMARY KEY AUTOINCREMENT, \
                      name TEXT NOT NULL)");
            query.exec();
        query.prepare("CREATE TABLE IF NOT EXISTS 'SKIPPED_FILTERS' ( \
                      id INTEGER PRIMARY KEY AUTOINCREMENT, \
                      name TEXT NOT NULL)");
            query.exec();

    }
}

QString DbInteraction::complexQuery(QString queryInput,QString fileName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");
    if(db.open()){
            QSqlDatabase db = QSqlDatabase::database("QSQLITE");
            QSqlQuery query;
            QString result;
            qDebug() << queryInput;
            qDebug() << fileName;
            query.prepare(queryInput);
            query.bindValue(":file",fileName);
            query.exec();
            if(!query.exec()){
                return query.lastError().text();
            }
            int id = query.record().indexOf("filename");
            while(query.next()){
                result += "--- id: "+(query.value("id").toString());
                result += " \n filename: "+(query.value("filename").toString());
                result += " \n last_modified: "+(query.value("last_modified").toString());
                result += " \n creationd_date: "+(query.value("creationd_date").toString());
                result += " \n size: "+(query.value("size").toString());
                result += " \n ext: "+(query.value("ext").toString());
                result += " \n type: "+(query.value("type").toString());
                result += " \n path: "+(query.value("path").toString());
                result += "---\n";

            }

            if(result==""){
                return "No data found with this query";
            }
            return result;
    }
    return "Error while opening database";


}






