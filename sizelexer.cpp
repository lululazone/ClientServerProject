
#include "sizelexer.h"

SizeLexer::SizeLexer()
{
    dialectMap["sizeSpec"] = {"M","K","G"};
    reSize_Spec.setPattern("^(?<value>\\d+)(?<type>(M|K|G))$");
    reSize_Spec_ex.setPattern("^BETWEEN (?<value1>\\d+)(?<type1>(M|K|G)) AND (?<value2>\\d+)(?<type2>(M|K|G))$");
}

QString SizeLexer::simpleQuery(QString type,QString value)
{
    QRegularExpressionMatch match = reSize_Spec.match(value);
    int val = match.captured("value").toInt();
    QString valType = match.captured("type");
    if(valType == "M"){
        val = val*1000*1000;
    }
    if(valType == "K"){
        val = val*1000;
    }
    if(valType == "G"){
        val = val*1000*1000*1000;
    }
    if(type=="SIZE"){
        return " AND size = '"+QString::number(val)+"'";
    }
    if(type=="MAX_SIZE"){
        return " AND size < '"+QString::number(val)+"'";
    }
    if(type=="MIN_SIZE"){
        return " AND size > '"+QString::number(val)+"'";
    }
    return "";

}
/*
 * Fais "A l'arrache par manque de temps..."
 *
 *
 * */
QString SizeLexer::harderQuery(QString type, QString value)
{
    QRegularExpressionMatch match = reSize_Spec_ex.match(value);
    int val1 = match.captured("value1").toInt();
    int val2 = match.captured("value2").toInt();
    QString valType1 = match.captured("type1");
    QString valType2 = match.captured("type2");
    if(valType1 == "M"){
        val1 = val1*1000*1000;
    }
    if(valType1 == "K"){
        val1 = val1*1000;
    }
    if(valType1 == "G"){
        val1 = val1*1000*1000*1000;
    }
    if(valType2 == "M"){
        val2 = val2*1000*1000;
    }
    if(valType2 == "K"){
        val2 = val2*1000;
    }
    if(valType2 == "G"){
        val2 = val2*1000*1000*1000;
    }
    if(type=="SIZE"){
        return " AND size > '"+QString::number(val1)+"' AND size < '"+QString::number(val2)+"'";
    }
    return "";
}

QString SizeLexer::buildQuery(QString value, QString type)
{
    QRegularExpressionMatch match = reSize_Spec.match(value);
    QRegularExpressionMatch match2 = reSize_Spec_ex.match(value);
    if(match.hasMatch()){
        return simpleQuery(type,value);
    }
    if(match2.hasMatch()){
        return harderQuery(type,value);
    }
    return "";
}

