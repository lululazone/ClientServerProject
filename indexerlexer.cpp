#include "indexerlexer.h"
#include "dbinteraction.h"
#include "indexerfsm.h"
#include "indexermanagerlexer.h"


IndexerLexer::IndexerLexer()
{
    dialectMap["indexer"] = {"INDEXER"};
    dialectMap["othertype"] = {"GET","ADD","PUSH","CLEAR"};



}

bool IndexerLexer::isIndexer(QString s)
{
    if(dialectMap["indexer"].contains(s)){
        return true;
    }
    return false;

}

bool IndexerLexer::isGet(QString s)
{
    if(dialectMap["othertype"].contains(s)){
        return true;
    }
    return false;

}

bool IndexerLexer::isAdd(QString s)
{
    if(dialectMap["othertype"].contains(s)){
        return true;
    }
    return false;

}

bool IndexerLexer::isPush(QString s)
{
    if(dialectMap["othertype"].contains(s)){
        return true;
    }
    return false;

}

bool IndexerLexer::isClear(QString s)
{
    if(dialectMap["othertype"].contains(s)){
        return true;
    }
    return false;
}

QString IndexerLexer::Tokenize(QStringList input,DbInteraction dbManager)
{
    IndexerFsm indexerFsm;
    IndexerManagerLexer indexerManagerLexer = *new IndexerManagerLexer();
    if(isIndexer(input.first())){
        return indexerFsm.manageIndexing(input,dbManager);
    }
    if(isAdd(input.first()) || isClear(input.first()) || isGet(input.first()) || isPush(input.first())){
        return indexerManagerLexer.Tokenize(input,dbManager);
    }
    return "No valid input for indexing statement";

}





