#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QCoreApplication>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QString>
#include <QDebug>
#include <QList>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include "ssh/sftpchannel.h"
#include "ssh/sshconnection.h"
#include "function.h"

class FileWidget : public QListWidget
{
    Q_OBJECT
public:
    FileWidget(QWidget *parent = 0);
    ~FileWidget();

    void initOperaMenu();   //右键菜单
    void openFileSystem();      //打开文件资源系统
    void setFileInfoList(const QList<QSsh::SftpFileInfo> &fiList);   //设置文件信息列表
    void refreshDirectory(const QList<QSsh::SftpFileInfo> &fiList);

signals:
    void openFileSystemClicked();
    void openClicked(QString fileName, QString fileType);
    void uploadClicked();
    void downloadClicked();
    void upClicked();
    void homeClicked();
    void newFolderClicked();
    void deleteClicked();
    void renameClicked();
    void refreshClicked();

public slots:
    void clickedOpen();//打开文件夹
    void clickedUpload();//上传
    void clickedDownload();//下载
    void clickedUp();//返回上一级
    void clickedHome();//返回主菜单
    void clickedNewFolder();//创建新文件夹
    void clickedDelete();//删除
    void clickedRename();//重命名
    void clickedRefresh();//刷新页面
    void customMenuView(QPoint pt);//右键菜单位置

    void handleItemDoubleClicked(QListWidgetItem *item);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QList<QSsh::SftpFileInfo> fileInfoList;

    QMenu    *m_pOperaMenu;
    QAction  *m_pActOpen;
    QAction  *m_pActUpload;
    QAction  *m_pActDownload;
    QAction  *m_pActUp;
    QAction  *m_pActHome;
    QAction  *m_pActRefresh;
    QAction  *m_pActNew;
    QAction  *m_pActDelete;
    QAction  *m_pActRename;
};

#endif // FILEWIDGET_H
