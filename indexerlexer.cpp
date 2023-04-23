#include "indexerlexer.h"


IndexerLexer::IndexerLexer()
{
    indexer = {"INDEXER","GET","ADD","PUSH","CLEAR"};
    Get_AddFilter = {"WHITELIST","BLACKLIST","FILTERS","SKIPPED_FILTERS"};
    indexerFilter = {"STATUS","START","PAUSE","STOP","RESUME"};



}

QString IndexerLexer::Tokenize(QStringList input)
{
    if(input.first()==indexer[0]){
        return indexerToken(input);
    }
    if(input.first()==indexer[1]){
        return getToken(input);
    }
    if(input.first()==indexer[2]){
        return addToken(input);
    }
    if(input.first()==indexer[3]){
        return pushToken(input);
    }
    if(input.first()==indexer[4]){
        return clearToken(input);
    }
    return "";

}

QString IndexerLexer::indexerToken(QStringList input)
{
    if(input.indexOf(indexerFilter[0]) == 1){
        return "File finder is running";
    }
    if(input.indexOf(indexerFilter[1]) == 1){
        return "File finder is started";
    }
    if(input.indexOf(indexerFilter[2]) == 1){
        return "File finder is paused";
    }
    if(input.indexOf(indexerFilter[3]) == 1){
        return "File finder is stopped";
    }
    if(input.indexOf(indexerFilter[4]) == 1){
        return "File finder is resumed";
    }
    return "Usage: INDEXER <STATUS|START|STOP|PAUSE|RESUME>";
}

QString IndexerLexer::getToken(QStringList input)
{
    if(input.indexOf(Get_AddFilter[0]) == 1){
        return "File finder get whitelist";
    }
    if(input.indexOf(Get_AddFilter[1]) == 1){
        return "File finder get blacklist";
    }
    if(input.indexOf(Get_AddFilter[2]) == 1){
        return "File finder get filters";
    }
    if(input.contains(Get_AddFilter[3]) ==1){
        return "File finder get skipped_filters";
    }
    return "Usage: GET <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS>";

}

QString IndexerLexer::addToken(QStringList input)
{
    if(input.size()<= 2){
        return "Usage: ADD <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folder to add|type_list_spec>";
    }

    if(input.indexOf(Get_AddFilter[0]) == 1){
        return "File finder add whitelist";
    }
    if(input.indexOf(Get_AddFilter[1]) == 1){
        return "File finder add blacklist";
    }
    if(input.indexOf(Get_AddFilter[2]) == 1){
        return "File finder add filters";
    }
    if(input.contains(Get_AddFilter[3]) ==1){
        return "File finder add skipped_filters";
    }
    return "Usage: ADD <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folder to add|type_list_spec>";

}

QString IndexerLexer::pushToken(QStringList input)
{
    if(input.size()<= 3){
        return "Usage: PUSH <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folder to add|type_list_spec> DONE";
    }
    if(input.indexOf(Get_AddFilter[0]) == 1){
        return "File finder push whitelist";
    }
    if(input.indexOf(Get_AddFilter[1]) == 1){
        return "File finder push blacklist";
    }
    if(input.indexOf(Get_AddFilter[2]) == 1){
        return "File finder push filters";
    }
    if(input.contains(Get_AddFilter[3]) ==1){
        return "File finder push skipped_filters";
    }
    return "Usage: PUSH <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS>  <folders to add|type_list_spec> DONE";

}

QString IndexerLexer::clearToken(QStringList input)
{
    if(input.indexOf(Get_AddFilter[0]) == 1){
        return "File finder clear whitelist";
    }
    if(input.indexOf(Get_AddFilter[1]) == 1){
        return "File finder clear blacklist";
    }
    if(input.indexOf(Get_AddFilter[2]) == 1){
        return "File finder clear filters";
    }
    if(input.contains(Get_AddFilter[3]) ==1){
        return "File finder clear skipped_filters";
    }
    return "Usage: CLEAR <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS>";

}



