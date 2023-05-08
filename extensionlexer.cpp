
#include "extensionlexer.h"

ExtensionLexer::ExtensionLexer()
{

}

QString ExtensionLexer::buildQuery(QString value)
{
    QString result = " AND (";
    QStringList extensionsList = value.split(",");
    for(int i =0; i<extensionsList.size();i++){
        result+=" ext = '"+extensionsList[i]+"'";
        if(i+1<extensionsList.size()){
            result+=" OR ";
        }
        else{
            result+=")";
        }
    }
    return result;
}

