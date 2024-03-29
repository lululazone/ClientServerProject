
#include "extensionlexer.h"
#include "qforeach.h"

ExtensionLexer::ExtensionLexer()
{

}

QString ExtensionLexer::buildQuery(QString value)
{
    QString result = " AND (";
    QStringList extensionsList;
    if(value.contains(",")){
        qDebug() << "contained";
        extensionsList = value.split(",");
    }
    else{
        extensionsList = value.split(" OR ");

    }

    qDebug() << extensionsList.size();
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

