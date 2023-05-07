
#ifndef DATELEXER_H
#define DATELEXER_H




#include "Lexer.h"
#include "qregularexpression.h"
class datelexer: public Lexer
{
public:
    datelexer();
    QString type;
    QString dateMatched;
    QRegularExpression reSimple;
    QRegularExpression reMonthPrecision;
    QRegularExpression reYearPrecision;
    QRegularExpression reTwoDigitYearPrecision;
    QString buildQuery(QStringList input, int cur,QString type);
    bool datePatternMatch(QString input, QRegularExpressionMatch match, int i);
    QString getDateMatched() const;
    QString dateMonthPrecision(QDateTime date);
    QString dateYearPrecision(QDateTime date, int type);
    void setDateMatched(const QString &newDateMatched, int type);
    QString getType() const;
    void setType(const QString &newType);
};

#endif // DATELEXER_H
