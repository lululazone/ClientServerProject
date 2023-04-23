#ifndef QUERYLEXER_H
#define QUERYLEXER_H

#include <Lexer.h>
#include <QStringList>



class QueryLexer : public Lexer
{
public:
    QueryLexer();
    QString Tokenize(QStringList input);
    QString optionToken(QStringList input);
    QStringList optionFilter;
    QString lastModified;
    QString created;
    QString maxSize;
    QString minSize;
    QString size;
    QString ext;
    QString type;
    QString dateLexer(QString element);
    QString sizeLexer(QString element);
};

#endif // QUERYLEXER_H
