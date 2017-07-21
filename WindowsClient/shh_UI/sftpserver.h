#ifndef SFTPSERVER_H
#define SFTPSERVER_H

#include <QWidget>
#include <ssh/sshconnection.h>
#include <ssh/sftpchannel.h>
#include <QObject>

namespace Ui {
class SftpServer;
}

class SftpServer : public QWidget
{
    Q_OBJECT

public:
    explicit SftpServer(QWidget *parent = 0);
    SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber = 0, QWidget *parent = 0);
    ~SftpServer();

public slots:
    void handleConnected();
    void handleError();
    void handleDisconnected();
    void handleChannelInitialized();
    void handleChannelInitializedFailed();
    void handleJobFinished();
    void handleFileInfo();
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
    
};

#endif // SFTPSERVER_H
