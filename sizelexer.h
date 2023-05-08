
#ifndef SIZELEXER_H
#define SIZELEXER_H




#include "Lexer.h"
#include "qregularexpression.h"
class SizeLexer: public Lexer
{
public:
    SizeLexer();
    QRegularExpression reSize_Spec;
    QRegularExpression reSize_Spec_ex;
    QString simpleQuery(QString type,QString value);
    QString harderQuery(QString type,QString value);
    QString buildQuery(QString value,QString type);
};

#endif // SIZELEXER_H
