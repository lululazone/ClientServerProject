
#include "dbinteraction.h"
#include "qthread.h"
#ifndef INDEXERFSM_H
#include <QMap>
#define INDEXERFSM_H
#include <QStringList>



class IndexerFsm : public QObject
{
public:
    DbInteraction dbManager;
    IndexerFsm();
    bool clientRequestedStopped;
    QMap<QString,QStringList> dialectIndexerMap;
    bool isStatus(QString input);
    bool isStart(QString input);
    bool isPause(QString input);
    bool isStop(QString input);
    bool isResume(QString input);
    QString statut();
    QString start();
    QString pause();
    QString stop();
    QString resume();
    QString manageIndexing(QStringList input,DbInteraction dbManager);


};

#endif // INDEXERFSM_H
