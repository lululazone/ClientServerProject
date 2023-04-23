#include "querylexer.h"

QueryLexer::QueryLexer()
{
    optionFilter = {"LAST_MODIFIED","CREATED","MAX_SIZE","MIN_SIZE","SIZE","EXT","TYPE"};

}

QString QueryLexer::Tokenize(QStringList input)
{
    if(input.size()<2){
        return "Usage: SEARCH <filename_part> [OPTIONS]";
    }
    return optionToken(input);
}

QString QueryLexer::optionToken(QStringList input)
{
    for (const QString &element : input) {
        if((element==optionFilter[0])){
            lastModified = dateLexer(element);
        }
        if((element==optionFilter[1])){
            created = dateLexer(element);
        }
        if((element==optionFilter[2])){
            maxSize = sizeLexer(element);
        }
        if((element==optionFilter[3])){
            minSize = sizeLexer(element);
        }
        if((element==optionFilter[4])){
            size = sizeLexer(element);
        }
    }
    return "";
}



QString QueryLexer::dateLexer(QString element){
    return element;//a continuer
}

QString QueryLexer::sizeLexer(QString element){
    return element;//a continuer
}


