#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QDataStream>

#include "sftpserver.h"
#include "../thread/threadobject.h"

class DownloadThread : public ThreadObject
{
    Q_OBJECT
public:
    DownloadThread(QSsh::SshConnection *connection, QString localPath, QString remotePath, QObject *parent = nullptr);
    ~DownloadThread();


    virtual void start();
    //暂停任务
    virtual void pause();
    //停止任务
    virtual void stop();
    //设置文件大小
//    virtual void setFileSize(qint64 fileSize);

    void run();
protected slots:
    void handleChannelInitialized();
    void handleChannelInitializationFailed(const QString &reason);
    void handleJobFinished(QSsh::SftpJobId id, const QString &error);
    void handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList);
    void handleChannelClosed();

private:

    //停掉当前任务
    void stopCurrenTask();
    //暂停当前任务
    void pauseCurrenTask();
    //初始化通道
    void initchannel();

    //定时检测当前任务进行状态
    void timerEvent(QTimerEvent *event);
    void startCheckCurrentProgressTimer();
    void stopCheckCurrentProgressTimer();

    int m_iTimerID;
    //增加一个TimerID的锁
    QMutex mutex;

    QFile m_file;
    //当前文件大小
    qint64 m_i64CurrentSize;
    //文件大小
    qint64 m_i64FileSize;
    //任务是否完成
    //包括暂停或者取消或者成功下载完
    bool m_bFinished;

    QSsh::SftpChannel::Ptr channel;
    QSsh::SshConnection *m_connection;

    QString localpath;
    QString remotepath;
signals:
    void currentTaskProgress(qint64);
};

#endif // DOWNLOADTHREAD_H
