#ifndef QUERYLEXER_H
#define QUERYLEXER_H

#include "dbinteraction.h"
#include "qdatetime.h"
#include <Lexer.h>
#include <QStringList>



class QueryLexer : public Lexer
{
public:
    QueryLexer();
    QString fileName;
    QString Tokenize(QStringList input,DbInteraction dbManager);
    bool isSearch(QStringList input);
    bool isOption(QStringList input);
    QString getResult(QString sqlQuery,DbInteraction dbManager);
    QString getFileName() const;
    QDateTime dateFixed;
    QDateTime dateBegin;
    QDateTime dateEnd;

    void setFileName(QStringList input);
};

#endif // QUERYLEXER_H
