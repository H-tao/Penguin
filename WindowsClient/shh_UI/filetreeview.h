#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCoreApplication>
#include <QWidget>
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
#include <QDropEvent>
#include <QDrag>
#include <QTreeView>
#include <QHeaderView>
#include <QModelIndex>
#include "ssh/sftpchannel.h"
#include "ssh/sshconnection.h"
#include "function.h"

class FileTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FileTreeView(QWidget *parent = 0);
    ~FileTreeView();
    void initialize();
    void initOperaMenu();   //右键菜单
    void openFileSystem();      //打开文件资源系统
    void refreshDirectory(const QList<QSsh::SftpFileInfo> &fiList);
    bool isFileExisted(QString fileName);
    void setThisFileInfo();
    void sortByColumn(int column, Qt::SortOrder order);

    QStandardItemModel *m_model;

signals:
    void openFileSystemClicked();
    void openClicked(QString fileName, QString fileType, QString fileSize);
    void uploadClicked();
    void downloadClicked(QString fileName, QString fileType, QString fileSize);
    void upClicked();
    void homeClicked();
    void newFolderClicked();
    void newFileClicked();
    void deleteClicked(QString fileName, QString fileType);
    void renameClicked(QString fileName);
    void refreshClicked();

public slots:
    void clickedOpen();//打开文件夹
    void clickedUpload();//上传
    void clickedDownload();//下载
    void clickedUp();//返回上一级
    void clickedHome();//返回主菜单
    void clickedNewFolder();//创建新文件夹
    void clickedNewFile();
    void clickedDelete();//删除
    void clickedRename();//重命名
    void clickedRefresh();//刷新页面
    void customMenuView(QPoint pt);//右键菜单位置
    void clickedSearch();

//    void handleItemDoubleClicked(QStandardItem *item);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QList<QSsh::SftpFileInfo> fileInfoList;

    QMenu    *m_pOperaMenu;
    QMenu    *m_pMarginMenu;
    QMenu    *m_pNewMenu;
    QAction  *m_pActOpen;
    QAction  *m_pActUpload;
    QAction  *m_pActDownload;
    QAction  *m_pActUp;
    QAction  *m_pActHome;
    QAction  *m_pActRefresh;
    QAction  *m_pActNewFile;
    QAction  *m_pActNewFolder;
    QAction  *m_pActDelete;
    QAction  *m_pActRename;

    QString thisFileName;
    QString thisFileType;
    QString thisFileSize;
    QString thisFilePermission;
    bool thisFilePermissionValid;
};

#endif // FILETREEVIEW_H
