
#ifndef OPTIONLEXER_H
#define OPTIONLEXER_H

#include <Lexer.h>





class OptionLexer : public Lexer
{
public:
    OptionLexer();

    QMap<QString,QRegularExpression> patternMatching;
    QString Tokenize(QStringList input,DbInteraction dbManager);
    bool dateMatch(QString input);
    bool datePatternMatch(QString input);

    QString isolateDate(QStringList inputList, int cur,QString result);
};

#endif // OPTIONLEXER_H
