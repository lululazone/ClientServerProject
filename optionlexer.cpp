
#include "optionlexer.h"
#include "datelexer.h"
#include "extensionlexer.h"
#include "qregularexpression.h"

OptionLexer::OptionLexer()
{
    dialectMap["date"] = {"LAST_MODIFIED","CREATED"};
    dialectMap["ext"] = {"EXT"};

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

            QString extResult = extLexer.buildQuery(input[i+1]);
            if(extResult == "error"){
                return "error";
            }
            sqlQuery+=extResult;


        }
    }

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



