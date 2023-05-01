
#ifndef ADDPUSHFSM_H
#define ADDPUSHFSM_H
#include <QMap>



#include "dbinteraction.h"
class AddPushFsm
{
public:
    AddPushFsm();
    QMap<QString,QStringList> dialectAddPushMap;
    QString manageAddOrPush(QStringList input,QString typeList,DbInteraction dbManager);
    bool isAdd(QString input);
    bool isPush(QString input);
    QString add(QString type, DbInteraction dbManager,QStringList input);
    QString push(QString type, DbInteraction dbManager,QStringList input);
};

#endif // ADDPUSHFSM_H
