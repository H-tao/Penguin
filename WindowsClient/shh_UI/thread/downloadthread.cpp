#include "downloadthread.h"

#include "../stable.h"
#include <QMutex>

DownloadThread::DownloadThread(QSsh::SshConnection *connection, QString localPath, QString remotePath, QObject *parent):
    ThreadObject(parent),
    localpath(localPath),
    remotepath(remotePath),
    m_connection(connection)
{
     m_iTimerID = 0;
    initchannel();
}

DownloadThread::~DownloadThread()
{
    m_bFinished = true;
}

void DownloadThread::start()
{
    startCheckCurrentProgressTimer();
}

void DownloadThread::pause()
{
    qDebug() << "DownloadThread::pause";
    //停止timer
    stopCheckCurrentProgressTimer();
    //暂停也是让这个线程退出，但是干得事情不一样
    pauseCurrenTask();
}

void DownloadThread::stop()
{
    qDebug() << "DownloadThread::stop";
    //停止timer
    stopCheckCurrentProgressTimer();
    //退出前需要处理一些东西
    stopCurrenTask();
}

void DownloadThread::stopCurrenTask()
{
    m_bFinished = true;

}

void DownloadThread::pauseCurrenTask()
{
    qDebug() << "pause";

    m_bFinished = true;
}


void DownloadThread::run()
{
    m_bFinished = false;
    qDebug() << "DownloadThread::run";
//    mutex.lock();
//    if(m_bFinished == false)
//        channelinit.wait(&mutex);
//    mutex.unlock();
//    m_bFinished = false;
//    channel->downloadFile(remotepath, localpath, QSsh::SftpOverwriteExisting);
    qDebug() << "DownloadThread::remotepath" <<remotepath;
    qDebug() << "DownloadThread::localpath" <<localpath;
    mutex.lock();
    if(m_bFinished == false)
        channelinit.wait(&mutex);
    mutex.unlock();
    QTest::qSleep(1000);
    qDebug() << "closechannel";
//    channel->closeChannel();
}
void DownloadThread::timerEvent(QTimerEvent *event)
{
    qDebug() << "DownloadThread::timerEvent(QTimerEvent *event)";
    if(event->timerId() == m_iTimerID)
    {
        qint64 progress = QFileInfo(localpath).size();
        qDebug() << "DownloadThread::localpath" << localpath;
        qDebug() << "QFileInfo(localpath).size()" << progress;
        emit currentTaskProgress(progress);
    }
    else
    {
        ThreadObject::timerEvent(event);
    }
}

void DownloadThread::startCheckCurrentProgressTimer()
{
    if(m_iTimerID)
        stopCheckCurrentProgressTimer();

    //1秒钟检测一次当前运行的状态
    m_iTimerID = startTimer(1000);
    qDebug() << "m_iTimerID  start" << m_iTimerID;
}

void DownloadThread::stopCheckCurrentProgressTimer()
{
    mutex.lock();
    if(m_iTimerID)
    {
        qDebug() << "m_iTimerID  end" << m_iTimerID;
        killTimer(m_iTimerID);
        m_iTimerID = 0;
    }
    mutex.unlock();
}
void DownloadThread::initchannel()
{
    if(!channel)
    {
        mutex.lock();
         if(!channel)
         {
         channel = m_connection->createSftpChannel();

         connect(channel.data(), SIGNAL(initialized()),
            this, SLOT(handleChannelInitialized()));
         connect(channel.data(), SIGNAL(initializationFailed(QString)),
            this, SLOT(handleChannelInitializationFailed(QString)));
         connect(channel.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
            this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)), Qt::DirectConnection);
         connect(channel.data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            this, SLOT(handleFileInfo(QSsh::SftpJobId, QList<QSsh::SftpFileInfo>)));
         connect(channel.data(), SIGNAL(closed()), this, SLOT(handleChannelClosed()));

         channel->initialize();
         mutex.unlock();
         qDebug() << "channel->initialize();";
         }
    }
}

void DownloadThread::handleChannelInitialized()
{
    qDebug() << "DownloadThread::handleChannelInitialized";
    // do something  initialize success
    mutex.lock();
    m_bFinished = true;
    channelinit.wakeAll();
    mutex.unlock();
    channel->downloadFile(remotepath, localpath, QSsh::SftpOverwriteExisting);
}

void DownloadThread::handleChannelInitializationFailed(const QString &reason)
{
    // initialize failed do something

}

void DownloadThread::handleJobFinished(QSsh::SftpJobId id, const QString &error)
{
    qDebug() << "DownloadThread::handleJobFinished";
}

void DownloadThread::handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList)
{
    qDebug() << "DownloadThread::handleFileInfo";
}

void DownloadThread::handleChannelClosed()
{
    qDebug() << "DownloadThread::handleChannelClosed";
}

