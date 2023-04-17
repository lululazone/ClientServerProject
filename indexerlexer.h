#ifndef INDEXERLEXER_H
#define INDEXERLEXER_H

#include <Lexer.h>
#include <QStringList>



class IndexerLexer : public Lexer
{
public:
    IndexerLexer();
    QString Tokenize(QStringList input);
    QStringList indexer;
    QStringList Get_AddFilter;
    QStringList indexerFilter;
    QString indexerToken(QStringList input);
    QString getToken(QStringList input);
    QString addToken(QStringList input);
    QString pushToken(QStringList input);
    QString clearToken(QStringList input);
};

#endif // INDEXERLEXER_H
