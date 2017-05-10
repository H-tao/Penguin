#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T18:47:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shh_UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    EnhancedLabel/enhancedlabel.cpp \
    FileSystemWidget/filesystemwidget.cpp \
    filesystem.cpp

HEADERS  += mainwindow.h \
    EnhancedLabel/enhancedlabel.h \
    FileSystemWidget/filesystemwidget.h \
    filesystem.h

FORMS    += mainwindow.ui \
    dialog.ui \
    feature.ui \
    filetransfer.ui \
    useloginauthentication.ui \
    new.ui \
    EnhancedLabel/enhancedlabel.ui \
    FileSystemWidget/filesystemwidget.ui \
    filesystem.ui

RESOURCES += \
    filetype.qrc
