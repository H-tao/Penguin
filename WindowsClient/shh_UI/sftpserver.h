#ifndef SFTPSERVER_H
#define SFTPSERVER_H

#include <QWidget>
#include <QQueue>
#include "tabpage.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QFileDialog>
#include <QtTest/QTest>
#include <QFileInfo>
#include <QList>
#include "ssh/sshconnection.h"
#include "ssh/sftpchannel.h"
#include "filetreeview.h"
#include "function.h"
#include "qfiletreeview.h"
#include "mainwindow.h"
#include "filenamedialog.h"

class MainWindow;

using Channel_Type = typename QSsh::SftpChannel::Ptr;

namespace Ui {
class SftpServer;
}

typedef struct
{
    QStandardItem *it;
    QString strPath;
}QueueItem;

class SftpServer : public QWidget
{
    Q_OBJECT

public:
    int serverNum;
    QString m_shellPath;
    MainWindow *m_mainWindow;

    SftpServer(QWidget *parent = 0);
    explicit SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber = 0,
                        QWidget *parent = 0, TabPage *p = 0, MainWindow *mainWindow = 0);
    ~SftpServer();

    void openTreeView();
    void initPage();
    void initTreeView();
    void initProgressDialog();
    void upload(QString localPath);

public slots:
    void handleConnected();
    void handleError();
    void handleDisconnected();
    void handleChannelInitialized();
    void  handleChannelInitializationFailed(const QString &reason);
    void handleJobFinished(QSsh::SftpJobId id, const QString &error);
    void handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList);
    void handleChannelClosed();
    void getNextLevelList(QString path);

    void handleOpenFileWidgetClicked();
    void handleOpenClicked(const QString &fileName, const QString &fileType, const QString &fileSize);
    void handleUpClicked();
    void handleHomeClicked();
    void handleDownloadClicked(const QString &fileName, const QString &fileType, const QString &fileSize);
    void handleRefreshClicked();
    void handleDeleteClicked(const QString &fileName, const QString &fileType);
    void handleUploadClicked();
    void handleRenameClicked(const QString &fileName);
    void handleNewFolderClicked();
    void handleNewFileClicked();

    void lineEditChanged(QString pathChanged);

    Channel_Type *createNewChannel(Channel_Type *channel);

private:
    Ui::SftpServer *ui;

    QSsh::SshConnection *m_connection;
    QSsh::SftpChannel::Ptr m_channel;

    QString m_currentPath;
    quint64 m_currentSize;
    QString m_currentLocalFilePath;
    QString m_currentRemoteFilePath;
    QString m_selectName;

    enum JobType{
        JobUnknow, JobStatFile, JobListDir, JobCreateDir, JobRemoveDir, JobRemoveFile, JobRename,
        JobCreateFile, JobCreateLink, JobUploadFile, JobDownloadFile, JobUploadDir, JobDownloadDir
    };
    JobType m_jobType;
    QSsh::SftpJobId m_jobListDirId;
    QSsh::SftpJobId m_jobUploadId;

//    FileTreeView *fileTree;
    QFileTreeView *m_treeView;
    TabPage *page;
    QProgressDialog *m_progress;
    int m_timer;

    enum WorkWidget{
        WorkFileWidget, WorkFileTreeView
    };
    WorkWidget m_workWidget;

    QStandardItemModel *m_treeItemModel;
    QStandardItem *m_currentItem;
    QStandardItem *item;
    QQueue<QueueItem *> items;
    QList<QSsh::SftpChannel::Ptr> channels;
    QSsh::SftpChannel::Ptr channel_2;
};

#endif // SFTPSERVER_H
