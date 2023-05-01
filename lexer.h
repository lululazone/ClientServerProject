#ifndef LEXER_H
#define LEXER_H

#include "dbinteraction.h"
#include "errormanager.h"
#include "qtextedit.h"
#include <QString>



class Lexer
{
public:

    Lexer();
    QMap<QString,QStringList> dialectMap;
    QString Tokenize(QString input,ErrorManager error,DbInteraction dbManager);
    bool isIndexerState(QString input);
};

#endif // LEXER_H
