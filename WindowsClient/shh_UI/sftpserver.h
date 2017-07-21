#ifndef SFTPSERVER_H
#define SFTPSERVER_H

#include <QWidget>
#include <ssh/sshconnection.h>
#include <ssh/sftpchannel.h>
#include <filetreeview.h>
#include "function.h"

namespace Ui {
class SftpServer;
}

class SftpServer : public QWidget
{
    Q_OBJECT

public:
    SftpServer(QWidget *parent = 0);
    explicit SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber = 0, QWidget *parent = 0);
    ~SftpServer();

public slots:
    void handleConnected();
    void handleError();
    void handleDisconnected();
    void handleChannelInitialized();
    void handleChannelInitializationFailed(const QString &renson);
    void handleJobFinished(QSsh::SftpJobId id, const QString &error);
    void handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList);
    void handleChannelClosed();

private:
    int serverNum;
    Ui::SftpServer *ui;
    
    QSsh::SshConnection *m_connection;
    QSsh::SftpChannel::Ptr m_channel;
    
    QString m_currentPath;
    
    enum JobType{
        JobUnknow, JobStatFile, JobListDir, JobCreateDir, JobRemoveDir, JobRemoveFile, JobRename,
        JobCreateFile, JobCreateLink, JobUploadFile, JobDownloadFile, JobUploadDir, JobDownloadDir
    };
    JobType m_jobType;
    QSsh::SftpJobId m_jobListDirId;
    QSsh::SftpJobId m_jobUploadId;
    
    FileTreeView *fileTree;

};

#endif // SFTPSERVER_H
