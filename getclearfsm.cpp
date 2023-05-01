
#include "getclearfsm.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"
#include "qsqlrecord.h"

GetClearFsm::GetClearFsm()
{
    dialectGetClearMap["get"] = {"GET"};
    dialectGetClearMap["clear"] = {"CLEAR"};
}

QString GetClearFsm::manageGetOrClear(QStringList input,QString typeList,DbInteraction dbManager)
{
    if(input.size()<2){
        return "GET <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> or CLEAR <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS>";
    }
    if(isGet(input.first())){
        return get(typeList,dbManager);
    }
    if(isClear(input.first())){
        return clear(typeList,dbManager);
    }
    return "No valid input for Get or Clear command";

}

bool GetClearFsm::isGet(QString s)
{
    if(dialectGetClearMap["get"].contains(s)){
        return true;
    }
    return false;
}

bool GetClearFsm::isClear(QString s)
{
    if(dialectGetClearMap["clear"].contains(s)){
        return true;
    }
    return false;
}

QString GetClearFsm::get(QString type,DbInteraction dbManager)
{
    QStringList tableList = dbManager.getTable(type);
    if(tableList.isEmpty()){
        return "No data found with this query";
    }
    else{
        QString result="";
        foreach (QString item, tableList) {
            result += item + "\n";
        }
        return result;
    }

}

QString GetClearFsm::clear(QString type,DbInteraction dbManager)
{
    return dbManager.deleteTable(type);
}
