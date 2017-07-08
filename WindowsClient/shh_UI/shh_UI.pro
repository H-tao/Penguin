#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T18:47:52
#
#-------------------------------------------------

QT       += core gui network
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
<<<<<<< HEAD
    shell.cpp
=======
    shell.cpp \
    newconnection.cpp
>>>>>>> e50478e7ee76e0782b519896a38ec4589b4a9357


HEADERS  += mainwindow.h \
    FileSystemWidget/filewidget.h \
    ShellTextEdit/shelltextedit.h \
<<<<<<< HEAD
    shell.h
=======
    shell.h \
    newconnection.h
>>>>>>> e50478e7ee76e0782b519896a38ec4589b4a9357


FORMS    += mainwindow.ui \
    dialog.ui \
    feature.ui \
    filetransfer.ui \
    useloginauthentication.ui \
    newconnection.ui

RESOURCES += \
    filetype.qrc

include(../qssh.pri) ## Required for IDE_LIBRARY_PATH and qtLibraryName
LIBS += -L$$IDE_LIBRARY_PATH -l$$qtLibraryName(Botan) -l$$qtLibraryName(QSsh)
