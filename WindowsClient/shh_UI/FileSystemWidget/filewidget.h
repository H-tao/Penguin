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

class FileWidget : public QListWidget
{
public:
    FileWidget(QWidget *parent);
    ~FileWidget();

    void openFileSystem();      //打开文件资源系统
    void setFileInfoList(QFileInfoList infoList);   //设置文件信息列表
    QIcon getFileIcon(const QString &suffix) const; //获取平台图标
    void Working();

private:
    QFileInfoList fileInfoList;

};

#endif // FILEWIDGET_H
