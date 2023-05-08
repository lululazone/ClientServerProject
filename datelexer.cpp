
#include "datelexer.h"
#include "qdatetime.h"
#include "qregularexpression.h"

datelexer::datelexer()
{
    reSimple.setPattern("^(?<date>\\d\\d)/(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
    reMonthPrecision.setPattern("^(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
    reYearPrecision.setPattern("^(?<year>\\d\\d\\d\\d)$");
    reTwoDigitYearPrecision.setPattern("^(?<year>\\d\\d)$");
    reSinceLast.setPattern("^(SINCE LAST ?<val1> (?<type1>MINUTES|HOURS|DAYS|MONTHS|YEAR) ?<val2> (?<type2>MINUTES|HOURS|DAYS|MONTHS|YEAR) AGO)$");
    reBetweenAnd.setPattern("^(BETWEEN ?<date1> AND ?<date2> $");

}

QString datelexer::dateBuilder(QString date)
{
    QRegularExpressionMatch match = reSimple.match(date);
    if(match.hasMatch()){
        return "'"+match.captured("date")+"/"+match.captured("month")+"/"+match.captured("year")+"'";
    }
    match = reMonthPrecision.match(date);
    if(match.hasMatch()){
        return "'01/"+match.captured("month")+"/"+match.captured("year")+"'";
    }
    match = reYearPrecision.match(date);
    if(match.hasMatch()){
        return "'01/"+match.captured("month")+"/"+match.captured("year")+"'";
    }
}

QString datelexer::getComplexDateMatched() const
{
    return complexDateMatched;
}

void datelexer::setComplexDateMatched(QRegularExpressionMatch match, int i)
{
    if(i==0){
        QString val1 = match.captured("val1");
        QString val2 = match.captured("val2");
        QString type1 = match.captured("type1");
        QString type2 = match.captured("type2");
        qDebug() << val1 << val2 << type1 << type2;
        complexDateMatched="";
    }
    if(i==1){
        QString val1 = match.captured("date1");
        QString val2 = match.captured("date2");
    }
}

QString datelexer::getType() const
{
    return type;
}

void datelexer::setType(const QString &newType)
{
    if(newType == "LAST_MODIFIED"){
        type = "last_modified";
        return;
    }
    if(newType == "CREATED"){
        type ="creation_date";
        return;
    }
    type="";
}

QString datelexer::buildQuery(QStringList input, int cur,QString type)
{
    setType(type);
    QRegularExpressionMatch match = reSimple.match(input[cur]);
    if(datePatternMatch(input[cur],match,0)){
        qDebug() << "Returning Date";
        return getDateMatched();
    }
    QString inputComplex="";
    for(int i = cur; i<input.size();i++){
        inputComplex += input[i];
    }
    QRegularExpressionMatch matchComplex = reSinceLast.match(inputComplex);
    if(complexDatePatternMatch(inputComplex,matchComplex,0)){

    }
    return "error";
}

bool datelexer::datePatternMatch(QString input,QRegularExpressionMatch match,int i)
{

    if(match.hasMatch()){
        qDebug() << "Match found";
        setDateMatched(match.captured(0),i);
        return true;
    }
    else{
        switch (i) {
            case 0: return datePatternMatch(input,reMonthPrecision.match(input),i+1);
                    break;
            case 1: return datePatternMatch(input,reYearPrecision.match(input),i+1);
                    break;
            case 2: return datePatternMatch(input,reTwoDigitYearPrecision.match(input),i+1);
                    break;
            default:return false;break;
        }
    }

}

bool datelexer::complexDatePatternMatch(QString input, QRegularExpressionMatch match, int i)
{
    if(match.hasMatch()){
        qDebug() << "Match found";
        setComplexDateMatched(match,i);
        return true;
    }
    else{
        switch(i){
        case 0: return complexDatePatternMatch(input,reBetweenAnd.match(input),i+1);
                break;
        case 1:return false;
                break;
        default: return false;
                break;

        }
    }
    return false;


}

QString datelexer::getDateMatched() const
{
    return dateMatched;
}

QString datelexer::dateMonthPrecision(QDateTime date){
    return getType()+" >= '"+date.toString("yyyy-MM-01")+"' AND "+getType()+" <= '"+date.toString("yyyy-MM-31")+"'";
}

QString datelexer::dateYearPrecision(QDateTime date,int type)
{
    switch(type){

        case 0: return getType()+" >= '"+date.toString("yyyy-01-01")+"' AND "+getType()+" <= '"+date.toString("yyyy-12-31")+"'";
                break;

        case 1: return getType()+" >= '"+date.toString("20yy-01-01")+"' AND "+getType()+" <= '"+date.toString("20yy-12-31")+"'";
                break;

        default:return "";break;

    }



}

void datelexer::setDateMatched(const QString &newDateMatched,int type)
{
    qDebug() << "Parsing date";
    qDebug() << "Date found "+newDateMatched;
    QDateTime date;
    switch(type){
        case 0: date = QDateTime::fromString(newDateMatched, "dd/MM/yyyy");
                dateMatched = getType()+" = '"+date.toString("yyyy-MM-dd")+"'";
                break;
        case 1:date = QDateTime::fromString(newDateMatched, "MM/yyyy");
               dateMatched = dateMonthPrecision(date);
               break;
        case 2:date = QDateTime::fromString(newDateMatched, "yyyy");
               dateMatched = dateYearPrecision(date,0);
               break;
        case 3:date = QDateTime::fromString(newDateMatched, "yy");
               dateMatched = dateYearPrecision(date,1);
               break;

        default:dateMatched ="";
                break;

    }



}

