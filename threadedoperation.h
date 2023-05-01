
#ifndef THREADEDOPERATION_H
#define THREADEDOPERATION_H



#include "dbinteraction.h"
#include "qobject.h"
#include "qthread.h"

class ThreadedOperation : public QObject
{
    Q_OBJECT
    QThread thread;
public:
    QSqlDatabase db;
    QString pathIn;
    ThreadedOperation();

public slots:
    void scanDisk(QString path,QSqlDatabase db);
    void threaded();


};

#endif // THREADEDOPERATION_H
