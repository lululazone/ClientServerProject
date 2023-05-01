
#include "addpushfsm.h"

AddPushFsm::AddPushFsm()
{
    dialectAddPushMap["add"] = {"ADD"};
    dialectAddPushMap["push"] = {"PUSH"};
    dialectAddPushMap["done"] = {"DONE"};
}

QString AddPushFsm::manageAddOrPush(QStringList input, QString typeList, DbInteraction dbManager)
{
    if(input.size()<2){
        return "ADD <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> or PUSH <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folders to add|type_list_spec> DONE";
    }
    if(isAdd(input.first())){
        return add(typeList,dbManager,input);
    }
    if(isPush(input.first())){
        return push(typeList,dbManager,input);
    }
    return "No valid input for Get or Clear command";
}

bool AddPushFsm::isAdd(QString input)
{
    return dialectAddPushMap["add"].contains(input);
}

bool AddPushFsm::isPush(QString input)
{
    return dialectAddPushMap["push"].contains(input);
}

QString AddPushFsm::add(QString type, DbInteraction dbManager,QStringList input)
{
    return dbManager.appendToTable(type,input[2]);
}

QString AddPushFsm::push(QString type, DbInteraction dbManager,QStringList input)
{
    QString result = "";
    foreach (QString value, input) {
        if(value != type && !dialectAddPushMap["push"].contains(value) && !dialectAddPushMap["done"].contains(value)){
            result += dbManager.appendToTable(type,value);
        }
    }
    return result;
}

