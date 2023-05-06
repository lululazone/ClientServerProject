
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
    QString buildQuery(QStringList input, int cur,QString type);
    bool datePatternMatch(QString input);
    QString getDateMatched() const;
    void setDateMatched(const QString &newDateMatched);
    QString getType() const;
    void setType(const QString &newType);
};

#endif // DATELEXER_H
