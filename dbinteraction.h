
#ifndef DBINTERACTION_H
#define DBINTERACTION_H




#include "qsqldatabase.h"
#include "qthread.h"
#include <QObject>
class DbInteraction
{
public:
    DbInteraction();
    QSqlDatabase db;
    QString deleteTable(QString type);
    QStringList getTable(QString type);
    void scanDisk(QString type);
    QString appendToTable(QString tableName, QString value);
    void initTables();

};

#endif // DBINTERACTION_H
