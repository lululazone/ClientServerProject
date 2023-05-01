
#ifndef INDEXERMANAGERLEXER_H
#define INDEXERMANAGERLEXER_H

#include "dbinteraction.h"
#include <Lexer.h>





class IndexerManagerLexer : public Lexer
{
public:
    IndexerManagerLexer();
    bool isError(QStringList s);
    bool isGetorClear(QString s);
    QString Tokenize(QStringList input,DbInteraction dbManager);
};

#endif // INDEXERMANAGERLEXER_H
