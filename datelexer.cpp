
#include "datelexer.h"
#include "qdatetime.h"
#include "qregularexpression.h"

datelexer::datelexer()
{
    reSimple.setPattern("^(?<date>\\d\\d)/(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
}

QString datelexer::getType() const
{
    return type;
}

void datelexer::setType(const QString &newType)
{
    type = newType;
}

QString datelexer::buildQuery(QStringList input, int cur,QString type)
{
    setType(type);
    if(datePatternMatch(input[cur])){
        qDebug() << "Returning Date";
        return getDateMatched();
    }
}

bool datelexer::datePatternMatch(QString input)
{
    QRegularExpressionMatch match = reSimple.match(input);
    if(match.hasMatch()){
        qDebug() << "Match found";
        setDateMatched(match.captured(0));
        return true;
    }
    return false;

}

QString datelexer::getDateMatched() const
{
    return dateMatched;
}

void datelexer::setDateMatched(const QString &newDateMatched)
{
    qDebug() << "Parsing date";
    qDebug() << "Date found "+newDateMatched;

    QDateTime date = QDateTime::fromString(newDateMatched, "dd/MM/yyyy");
    if (!date.isValid()) {
        date = QDateTime::fromString(newDateMatched, "MM/yyyy");
    }
    if (!date.isValid()) {
        date = QDateTime::fromString(newDateMatched, "yyyy");
    }
    if (!date.isValid()) {
        date = QDateTime::fromString(newDateMatched, "yy");
    }
    if (date.isValid()) {
        qDebug() << "Parse success";
        if(getType() == "LAST_MODIFIED"){
            dateMatched = "last_modified = '"+date.toString("yyyy-MM-dd")+"'";
            qDebug() << "Last modified type";
            return;
        }
        if(getType() == "CREATED"){
            dateMatched = "creation_date = '"+date.toString("yyyy-MM-dd")+"'";
            qDebug() << "Creation type";
            return;
        }

    }
    dateMatched ="";


}

