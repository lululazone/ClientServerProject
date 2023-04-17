#include "lexer.h"
#include "QStringList"
#include "errormanager.h"
#include <QueryLexer.h>
#include <IndexerLexer.h>

Lexer::Lexer()
{

}

QString Lexer::Tokenize(QString input,ErrorManager error)
{
    QStringList tokenList = input.split(" ");
    QString s = tokenList.first();
    IndexerLexer indexerLexer;
    QueryLexer queryLexer;

    if(s == "INDEXER" || s == "GET" || s == "ADD" || s == "PUSH"){
        indexerLexer = *new IndexerLexer();
        return indexerLexer.Tokenize(tokenList);
    }
    if(s == "SEARCH"){
        queryLexer = *new QueryLexer();
        return queryLexer.Tokenize(tokenList);
    }
    else{
        return error.sendError("Invalid Syntax");
    }

    return "";


}
