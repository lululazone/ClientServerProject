#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include "qtextedit.h"
#include <QString>

class ErrorManager
{
public:
    ErrorManager();
    QString sendError(QString error);
};

#endif // ERRORMANAGER_H
