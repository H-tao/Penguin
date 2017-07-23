#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T18:47:52
#
#-------------------------------------------------

QT       += core gui network
QT       += winextras

SUBDIRS = \
    src
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shh_UI
TEMPLATE = app

INCLUDEPATH = $$PWD/src/libs/ssh/

SOURCES += main.cpp\
        mainwindow.cpp \
    FileSystemWidget/filewidget.cpp \
    ShellTextEdit/shelltextedit.cpp \
    shell.cpp \
    newconnection.cpp\
    tabpage.cpp \
    sftpserver.cpp \
    filetreeview.cpp \
    function.cpp \
    qfiletreeview.cpp

HEADERS  += mainwindow.h \
    FileSystemWidget/filewidget.h \
    ShellTextEdit/shelltextedit.h \
    shell.h \
    newconnection.h \
    tabpage.h \
    sftpserver.h \
    filetreeview.h \
    function.h \
    qfiletreeview.h


FORMS    += mainwindow.ui \
    dialog.ui \
    feature.ui \
    filetransfer.ui \
    useloginauthentication.ui \
    newconnection.ui \
    tabpage.ui \
    sftpserver.ui

RESOURCES += \
    filetype.qrc

include(../qssh.pri) ## Required for IDE_LIBRARY_PATH and qtLibraryName
LIBS += -L$$IDE_LIBRARY_PATH -l$$qtLibraryName(Botan) -l$$qtLibraryName(QSsh)
