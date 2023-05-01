#ifndef INDEXERLEXER_H
#define INDEXERLEXER_H

#include "dbinteraction.h"
#include <Lexer.h>
#include <QStringList>



class IndexerLexer : public Lexer
{
public:
    IndexerLexer();
    bool isIndexer(QString s);
    bool isGet(QString s);
    bool isAdd(QString s);
    bool isPush(QString s);
    bool isClear(QString s);
    QMap<QString,QStringList> dialectMap;
    QString Tokenize(QStringList input,DbInteraction dbManager);
};

#endif // INDEXERLEXER_H
