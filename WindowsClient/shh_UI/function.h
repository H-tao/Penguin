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
QString getPermissions(QFile::Permissions permissions);
QString getSizeFromByte(quint64 iByte);
quint64 getSizeToByte(QString strSize);
QString getFileType(QString strName);
QString getFolderType();


#endif // FUNCTION_H
