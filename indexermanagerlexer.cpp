
#include "indexermanagerlexer.h"
#include "addpushfsm.h"
#include "getclearfsm.h"

IndexerManagerLexer::IndexerManagerLexer()
{
    dialectMap["getclear"] = {"GET","CLEAR"};
    dialectMap["addpush"] = {"ADD","PUSH"};
    dialectMap["listtype"] = {"WHITELIST","BLACKLIST","FILTERS","SKIPPED_FILTERS"};
}

bool IndexerManagerLexer::isError(QStringList s)
{
    return (!dialectMap["listtype"].contains(s[1]));
}

bool IndexerManagerLexer::isGetorClear(QString s)
{
    if(dialectMap["getclear"].contains(s)){
        return true;
    }
    return false;
}


QString IndexerManagerLexer::Tokenize(QStringList input,DbInteraction dbManager)
{
    GetClearFsm getClearFsm = *new GetClearFsm();
    AddPushFsm addPushFsm = *new AddPushFsm();
    if(input.size()<2){
        return "No valid input for GET,ADD,PUSH or CLEAR";
    }
    if(!isError(input)){
        if(isGetorClear(input.first())){
            return getClearFsm.manageGetOrClear(input,input[1],dbManager);
        }
        else{
            return addPushFsm.manageAddOrPush(input,input[1],dbManager);
        }

    }


    return "No valid input for GET,ADD,PUSH or CLEAR";




}
