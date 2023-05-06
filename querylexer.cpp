#include "querylexer.h"

#include <OptionLexer.h>

QueryLexer::QueryLexer()
{

}

QString QueryLexer::getFileName() const
{
    return fileName;
}

void QueryLexer::setFileName(QStringList input)
{
    fileName = "";
    for(int i = 1;i<input.size();i++){
        if(!dialectMap["options"].contains(input[i])){
            fileName+=input[i]+" ";
        }
        else{
            fileName.chop(1);
            return;
        }
    }
    fileName.chop(1);
}

QString QueryLexer::Tokenize(QStringList input,DbInteraction dbManager)
{
    setFileName(input);
    QString sqlQuery = "SELECT * FROM files WHERE filename = :file";
    if(input.size()<2){
        return "Usage: SEARCH <filename_part> [OPTIONS]";
    }
    if(isSearch(input)){
        if(isOption(input)){
            OptionLexer *optionLexer = new OptionLexer();
            sqlQuery += optionLexer->Tokenize(input,dbManager);
        }
    }

    return getResult(sqlQuery,dbManager);



}

bool QueryLexer::isSearch(QStringList input)
{
    return dialectMap["query"].contains(input.first());
}

bool QueryLexer::isOption(QStringList input)
{
    for(QString token : input){
        if(dialectMap["options"].contains(token))
            return true;

    }
    return false;
}

QString QueryLexer::getResult(QString sqlQuery, DbInteraction dbManager)
{
    return dbManager.complexQuery(sqlQuery,getFileName());
}


