
#include "optionlexer.h"
#include "datelexer.h"
#include "qregularexpression.h"

OptionLexer::OptionLexer()
{
    dialectMap["date"] = {"LAST_MODIFIED","CREATED"};

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

            sqlQuery += dlexer.buildQuery(input,y,input[i]);
            qDebug() << sqlQuery + " On iteration: "+QString::number(i);
        }
    }
    return sqlQuery;
}

bool OptionLexer::dateMatch(QString input)
{
    return dialectMap["date"].contains(input);
}



