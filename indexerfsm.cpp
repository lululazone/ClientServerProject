
#include "indexerfsm.h"
#include "qdiriterator.h"
#include "qmimedatabase.h"
#include "qsqlerror.h"
#include "qsqlquery.h"

IndexerFsm::IndexerFsm()
{
    dialectIndexerMap["statut"] = {"STATUS"};
    dialectIndexerMap["start"] = {"START"};
    dialectIndexerMap["stop"] = {"STOP"};
    dialectIndexerMap["pause"] = {"PAUSE"};
    dialectIndexerMap["resume"] = {"RESUME"};

}

bool IndexerFsm::isStatus(QString s)
{

    if(dialectIndexerMap["statut"].contains(s)){
        return true;
    }
    return false;
}

bool IndexerFsm::isStart(QString s)
{
    if(dialectIndexerMap["start"].contains(s)){
        return true;
    }
    return false;
}

bool IndexerFsm::isPause(QString s)
{
    if(dialectIndexerMap["pause"].contains(s)){
        return true;
    }
    return false;
}

bool IndexerFsm::isStop(QString s)
{
    if(dialectIndexerMap["stop"].contains(s)){
        return true;
    }
    return false;
}

bool IndexerFsm::isResume(QString s)
{
    if(dialectIndexerMap["resume"].contains(s)){
        return true;
    }
    return false;
}

QString IndexerFsm::statut()
{
    return "statut";
}

QString IndexerFsm::start()
{
    return "start";
}

QString IndexerFsm::pause()
{
    return "pause";
}

QString IndexerFsm::stop()
{
    return "stop";
}

QString IndexerFsm::resume()
{
    return "resume";
}

QString IndexerFsm::manageIndexing(QStringList input,DbInteraction dbMan)
{
    if(input.size()<2){
        return "INDEXER <STATUS|START|STOP|PAUSE|RESUME>";
    }
    if(isStatus(input[1])){
        return statut();
    }
    if(isStart(input[1])){
        return start();
    }
    if(isPause(input[1])){
        return pause();
    }
    if(isStop(input[1])){
        return stop();
    }
    if(isResume(input[1])){
        return resume();
    }
    return "INDEXER <STATUS|START|STOP|PAUSE|RESUME>";

}



