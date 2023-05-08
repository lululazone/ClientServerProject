#include "querylexer.h"

#include <OptionLexer.h>

QueryLexer::QueryLexer()
{
    dialectMap["options"] = {"LAST_MODIFIED","CREATED","MAX_SIZE","MIN_SIZE","SIZE","EXT"};
}

QString QueryLexer::getFileName()
{
    return fileName;
}

void QueryLexer::setFileName(QStringList input)
{
    QString temp="%";
    for(int i = 1;i<input.size();i++){
        if(!dialectMap["options"].contains(input[i])){
            qDebug() <<input[i];
            temp+=input[i]+" ";
        }
        else{
            temp.chop(1);

            temp+="%";
            fileName = temp;
            qDebug() << "Name of the file is "+fileName;
            return;
        }
    }
    temp.chop(1);
    temp+="%";
    fileName = temp;
    qDebug() << "Name of the file is "+fileName;

}

QString QueryLexer::Tokenize(QStringList input,DbInteraction dbManager)
{
    qDebug()<<input.size();
    setFileName(input);
    QString sqlQuery = "SELECT * FROM files WHERE filename LIKE :file";
    if(input.size()<2){
        return "Usage: SEARCH <filename_part> [OPTIONS]";
    }
    if(isSearch(input)){
        if(isOption(input)){
            OptionLexer *optionLexer = new OptionLexer();
            QString optionResult = optionLexer->Tokenize(input,dbManager);
            if(optionResult != "error"){
                sqlQuery += optionResult;
            }
            else{
                return "Error while trying to fetch options, ensure that you provided a correct syntax";
            }

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


