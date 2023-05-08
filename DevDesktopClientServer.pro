QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addpushfsm.cpp \
    client.cpp \
    datelexer.cpp \
    dbinteraction.cpp \
    errormanager.cpp \
    extensionlexer.cpp \
    getclearfsm.cpp \
    indexerfsm.cpp \
    indexerlexer.cpp \
    indexermanagerlexer.cpp \
    lexer.cpp \
    main.cpp \
    mainwindow.cpp \
    optionlexer.cpp \
    querylexer.cpp \
    server.cpp \
    sizelexer.cpp \
    threadedoperation.cpp \
    typelexer.cpp

HEADERS += \
    addpushfsm.h \
    client.h \
    datelexer.h \
    dbinteraction.h \
    errormanager.h \
    extensionlexer.h \
    getclearfsm.h \
    indexerfsm.h \
    indexerlexer.h \
    indexermanagerlexer.h \
    lexer.h \
    mainwindow.h \
    optionlexer.h \
    querylexer.h \
    server.h \
    sizelexer.h \
    threadedoperation.h \
    typelexer.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    DevDesktopClientServer_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    search.qrc \
    butterfly.qrc


QT += network

DISTFILES += \
    ../../../../Pictures/search-interface-symbol.png
