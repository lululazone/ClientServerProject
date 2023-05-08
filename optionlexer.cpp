
#include "optionlexer.h"
#include "datelexer.h"
#include "extensionlexer.h"
#include "qregularexpression.h"
#include "sizelexer.h"
#include "typelexer.h"

OptionLexer::OptionLexer()
{
    dialectMap["date"] = {"LAST_MODIFIED","CREATED"};
    dialectMap["ext"] = {"EXT"};
    dialectMap["options"] = {"LAST_MODIFIED","CREATED","MAX_SIZE","MIN_SIZE","SIZE","EXT","TYPE"};
    dialectMap["type"] = {"TYPE"};
    dialectMap["size"] = {"MAX_SIZE","MIN_SIZE","SIZE"};

}






QString OptionLexer::Tokenize(QStringList input, DbInteraction dbManager)
{
    QString sqlQuery ="";
    for(int i =2; i<input.size();i++){

        if(dateMatch(input[i])){
            sqlQuery += " AND ";
            QString inputPattern="";
            int y= i+1;
            datelexer dlexer;
            QString dateResult = dlexer.buildQuery(input,y,input[i]);
            if(dateResult != "error"){
                sqlQuery += dateResult;
            }
            else{
                return "error";
            }
            qDebug() << sqlQuery + " On iteration: "+QString::number(i);
        }
        if(isExt(input[i])){
            ExtensionLexer extLexer;
            QString parameter = catchParameter(input,i+1);
            QString extResult = extLexer.buildQuery(parameter);
            if(extResult == "error"){
                return "error";
            }
            sqlQuery+=extResult;
        }
        if(isType(input[i])){
            TypeLexer typeLexer;
            QString parameter = catchParameter(input,i+1);
            QString extResult = typeLexer.buildQuery(parameter);
            if(extResult == "error"){
                return "error";
            }
            sqlQuery+=extResult;
        }
        if(isSize(input[i])){
            SizeLexer sizeLexer;
            QString parameter = catchParameter(input,i+1);
            QString sizeResult = sizeLexer.buildQuery(parameter,input[i]);
            if(sizeResult == "error"){
                return "error";
            }
            sqlQuery+=sizeResult;
        }
    }
    qDebug() << sqlQuery;
    return sqlQuery;
}

bool OptionLexer::dateMatch(QString input)
{
    return dialectMap["date"].contains(input);
}

bool OptionLexer::isExt(QString input)
{
    return dialectMap["ext"].contains(input);
}

bool OptionLexer::isType(QString input)
{
    return dialectMap["type"].contains(input);
}

bool OptionLexer::isSize(QString input)
{
    return dialectMap["size"].contains(input);
}

QString OptionLexer::catchParameter(QStringList input,int cur)
{
    QString result="";
    for(int i=cur;i<input.size();i++){
        if(!dialectMap["options"].contains(input[i])){

            result += input[i];
            result += " ";
        }
        else{
            result.chop(1);
            return result;
        }
    }
    result.chop(1);
    return result;

}



