#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QCoreApplication>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QIcon>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QString>
#include <QDebug>
#include <QList>
#include <ssh/sftpchannel.h>
#include <ssh/sshconnection.h>
#include "function.h"

class FileWidget : public QListWidget
{
    Q_OBJECT
public:
    FileWidget(QWidget *parent = 0);
    ~FileWidget();

    void openFileSystem();      //打开文件资源系统
    void setFileInfoList(const QList<QSsh::SftpFileInfo> &fiList);   //设置文件信息列表
    void refreshDirectory(const QList<QSsh::SftpFileInfo> &fiList);

signals:
    void openClicked();

public slots:

private:
    QList<QSsh::SftpFileInfo> fileInfoList;
};

#endif // FILEWIDGET_H
