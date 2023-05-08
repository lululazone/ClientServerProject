
#ifndef EXTENSIONLEXER_H
#define EXTENSIONLEXER_H




#include "Lexer.h"
class ExtensionLexer:public Lexer
{
public:
    ExtensionLexer();
    QString buildQuery(QString value);

};

#endif // EXTENSIONLEXER_H
