
#ifndef GETCLEARFSM_H
#define GETCLEARFSM_H
#include "dbinteraction.h"
#include "qsqldatabase.h"
#include <QStringList>
#include <QMap>


class GetClearFsm
{
public:
    GetClearFsm();
    QMap<QString,QStringList> dialectGetClearMap;
    QString manageGetOrClear(QStringList input,QString typeList,DbInteraction dbManager);
    bool isGet(QString input);
    bool isClear(QString input);
    QString get(QString type, DbInteraction dbManager);
    QString clear(QString type, DbInteraction dbManager);
};

#endif // GETCLEARFSM_H
