#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QList>

#include <qDebug>
#include "../stable.h"
#include "ssh/sshconnection.h"
#include "ssh/sftpchannel.h"

QT_BEGIN_NAMESPACE
class QWidget;
class ManageListWidget;
class QVBoxLayout;
class QListWidgetItem;
QT_END_NAMESPACE

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    DownloadWidget(QWidget *parent = nullptr);
    explicit DownloadWidget(QWidget *parent = nullptr, QSsh::SshConnection *connection = NULL);
    ~DownloadWidget();

    void addTask(QString localPath, QFile *file, QString remotePath, qint64 filesize);

    int getMaxTaskNumbers() const;
    void setMaxTaskNumbers(int iMaxTaskNumbers);

private:
    bool contains(QFile *file);

    QSsh::SshConnection *m_connection;
    QSsh::SftpChannel::Ptr m_channel;
    ManageListWidget *m_pListWidget;
    QVBoxLayout *m_pVBosLayout;

    //将具体的界面加到这个队列里
    QList<QWidget *> *m_pRunningTask;
    QList<QWidget *> *m_pWaittingTask;

    //最大任务数量
    int m_iMaxTaskNumbers;

protected slots:
    void startTask(QListWidgetItem *item);
    void pauseTask(QListWidgetItem *item);
    void stopTask(QListWidgetItem *item);
    void finishedTask(QListWidgetItem *item);

signals:
    void finished(QFile *);
};

