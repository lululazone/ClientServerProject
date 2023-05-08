
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
    QString complexDateMatched;
    QRegularExpression reSimple;
    QRegularExpression reMonthPrecision;
    QRegularExpression reYearPrecision;
    QRegularExpression reTwoDigitYearPrecision;
    QRegularExpression reSinceLast;
    QRegularExpression reBetweenAnd;
    QString dateBuilder(QString date);
    QString buildQuery(QStringList input, int cur,QString type);
    bool datePatternMatch(QString input, QRegularExpressionMatch match, int i);
    bool complexDatePatternMatch(QString input, QRegularExpressionMatch match, int i);
    QString getDateMatched() const;
    QString dateMonthPrecision(QDateTime date);
    QString dateYearPrecision(QDateTime date, int type);
    void setDateMatched(const QString &newDateMatched, int type);
    QString getType() const;
    void setType(const QString &newType);
    QString getComplexDateMatched() const;
    void setComplexDateMatched(QRegularExpressionMatch match, int i);
};

#endif // DATELEXER_H
