
#include "datelexer.h"
#include "qdatetime.h"
#include "qregularexpression.h"

datelexer::datelexer()
{
    reSimple.setPattern("^(?<date>\\d\\d)/(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
    reMonthPrecision.setPattern("^(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
    reYearPrecision.setPattern("^(?<year>\\d\\d\\d\\d)$");
    reTwoDigitYearPrecision.setPattern("^(?<year>\\d\\d)$");

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
            case 0: return datePatternMatch(input,match=reMonthPrecision.match(input),i+1);
                    break;
            case 1: return datePatternMatch(input,match=reYearPrecision.match(input),i+1);
                    break;
            case 2: return datePatternMatch(input,match=reTwoDigitYearPrecision.match(input),i+1);
                    break;
            default:return false;break;
        }
    }

}

QString datelexer::getDateMatched() const
{
    return dateMatched;
}

QString datelexer::dateMonthPrecision(QDateTime date){
    return getType()+" >= '"+date.toString("yyyy-MM-01")+"' AND last_modified <= '"+date.toString("yyyy-MM-31")+"'";
}

QString datelexer::dateYearPrecision(QDateTime date,int type)
{
    switch(type){

        case 0: return getType()+" >= '"+date.toString("yyyy-01-01")+"' AND last_modified <= '"+date.toString("yyyy-12-31")+"'";
                break;

        case 1: return getType()+" >= '"+date.toString("yyyy-01-01")+"' AND last_modified <= '"+date.toString("yyyy-12-31")+"'";
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

