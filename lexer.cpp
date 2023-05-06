#include "lexer.h"
#include "QStringList"
#include "dbinteraction.h"
#include "errormanager.h"
#include "qsqldatabase.h"
#include <QueryLexer.h>
#include <IndexerLexer.h>




Lexer::Lexer()
{
    dialectMap["query"] = {"SEARCH"};
    dialectMap["indexer"] = {"INDEXER"};
    dialectMap["othertype"] = {"GET","ADD","PUSH","CLEAR"};
    dialectMap["options"] = {"LAST_MODIFIED","CREATED","MAX_SIZE","MIN_SIZE","SIZE","EXT"};

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./qtdb.db");

}


QString Lexer::Tokenize(QString input,ErrorManager error,DbInteraction dbManager)
{
    QStringList tokenList = input.split(" ");
    QString s = tokenList.first();
    IndexerLexer indexerLexer;
    QueryLexer queryLexer;

    if(isIndexerState(s)){
        indexerLexer = *new IndexerLexer();
        qDebug() << "Allo";
        return indexerLexer.Tokenize(tokenList,dbManager);
    }
    if(isQueryState(s)){
        return queryLexer.Tokenize(tokenList,dbManager);
    }
    return "Waiting for an input...";



}

bool Lexer::isIndexerState(QString s)
{
    if(dialectMap["indexer"].contains(s) || dialectMap["othertype"].contains(s)){
        return true;
    }
    return false;
}

bool Lexer::isQueryState(QString input)
{
    if(dialectMap["query"].contains(input)){
        return true;
    }
    return false;
}
