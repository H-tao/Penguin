#ifndef FUNCTION_H
#define FUNCTION_H

#include <QIcon>
#include <QPixmap>
#include <QtWinExtras/QtWin>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QDir>
#include <QCoreApplication>


QIcon getFileIcon(const QString name);
QIcon getFolderIcon();

#endif // FUNCTION_H
