
#ifndef TYPELEXER_H
#define TYPELEXER_H




#include "Lexer.h"
class TypeLexer : public Lexer
{
public:
    TypeLexer();
    QString buildQuery(QString value);
};

#endif // TYPELEXER_H
