#include "lexer.h"
#include "QStringList"
#include "dbinteraction.h"
#include "errormanager.h"
#include "qsqldatabase.h"
#include <QueryLexer.h>
#include <IndexerLexer.h>




Lexer::Lexer()
{
    dialectMap["indexer"] = {"INDEXER","GET","ADD","PUSH","CLEAR"};
    dialectMap["query"] = {"SEARCH"};
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
        return indexerLexer.Tokenize(tokenList,dbManager);
    }
    return queryLexer.Tokenize(tokenList,dbManager);


}

bool Lexer::isIndexerState(QString s)
{
    if(dialectMap["indexer"].contains(s)){
        return true;
    }
    return false;
}
