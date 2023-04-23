#ifndef LEXER_H
#define LEXER_H

#include "errormanager.h"
#include "qtextedit.h"
#include <QString>



class Lexer
{
public:
    Lexer();
    QString Tokenize(QString input,ErrorManager error);
};

#endif // LEXER_H
