#include "downloadwidget.h"

#include "../stable.h"

#include "../download/downloadlist.h"
#include "../download/managelistwidget.h"

DownloadWidget::DownloadWidget(QWidget *parent, QSsh::SshConnection *connection):
    QWidget(parent),
m_connection(connection),
m_pListWidget(nullptr),
m_pVBosLayout(nullptr),
m_pWaittingTask(nullptr)
{
    m_pListWidget = new ManageListWidget;
    m_pVBosLayout = new QVBoxLayout(this);
    m_pVBosLayout->setContentsMargins(0, 0, 0, 0);
    m_pVBosLayout->setSpacing(0);
    m_pRunningTask = new QList<QWidget *>;
    m_pWaittingTask = new QList<QWidget *>;

    m_pVBosLayout->addWidget(m_pListWidget);
    qDebug() << "DownloadWidget :: connection" << m_connection;

    setMaxTaskNumbers(5);
}

DownloadWidget::~DownloadWidget()
{

    delete m_pListWidget;
    m_pListWidget = nullptr;

    delete m_pVBosLayout;
    m_pVBosLayout = nullptr;

    //这里需要做系统操作，暂时没写
    m_pRunningTask->clear();
    delete m_pRunningTask;
    m_pRunningTask = nullptr;

    m_pWaittingTask->clear();

    delete m_pWaittingTask;
    m_pWaittingTask = nullptr;
}

void DownloadWidget::addTask(QString localPath, QFile *file, QString remotePath, qint64 filesize)
{
    qDebug() << "addTask";

//    if (!contains(file))
//    {
//        qDebug() << "contains(file)";
        DownloadList *downloadList = new DownloadList(m_pListWidget, this, m_connection);

        connect(downloadList, SIGNAL(start(QListWidgetItem*)), this, SLOT(startTask(QListWidgetItem*)));
        connect(downloadList, SIGNAL(pause(QListWidgetItem*)), this, SLOT(pauseTask(QListWidgetItem*)));
        connect(downloadList, SIGNAL(stop(QListWidgetItem*)), this, SLOT(stopTask(QListWidgetItem*)));
        connect(downloadList, SIGNAL(finished(QListWidgetItem*)), this, SLOT(finishedTask(QListWidgetItem*)));

        m_pListWidget->setItemWidget(downloadList->getListWidgetItem(), downloadList);

        downloadList->setFile(file,localPath,remotePath,filesize);
//    }
//    else
//    {
//        //任务已存在，先不处理
//        delete file;
//    }

}

int DownloadWidget::getMaxTaskNumbers() const
{
    return m_iMaxTaskNumbers;
}

void DownloadWidget::setMaxTaskNumbers(int iMaxTaskNumbers)
{
    m_iMaxTaskNumbers = iMaxTaskNumbers;
}

bool DownloadWidget::contains(QFile *file)
{
    QListWidgetItem *item;
    DownloadList *widget;
    for (int i = 0; i < m_pListWidget->count(); ++i)
    {
        item = m_pListWidget->item(i);
        widget = (DownloadList *)m_pListWidget->itemWidget(item);

        if (!widget)
            continue;

        if (widget->getFile() == file)
            return true;
    }
    return false;
}


void DownloadWidget::startTask(QListWidgetItem *item)
{
    DownloadList *widget = (DownloadList *)m_pListWidget->itemWidget(item);

    if (m_pRunningTask->count() < m_iMaxTaskNumbers)
    {
        m_pRunningTask->append(widget);
        widget->startDownload();
    }
    else
        m_pWaittingTask->append(widget);

}

void DownloadWidget::pauseTask(QListWidgetItem *item)
{
    QWidget *widget = m_pListWidget->itemWidget(item);
    if (!m_pRunningTask->isEmpty() && m_pRunningTask->contains(widget))
    {
        m_pRunningTask->removeOne(widget);
    }

    if (!m_pWaittingTask->isEmpty() && m_pWaittingTask->contains(widget))
    {
        m_pWaittingTask->removeOne(widget);
    }
}

void DownloadWidget::stopTask(QListWidgetItem *item)
{
    QWidget *widget = m_pListWidget->itemWidget(item);

    if (!m_pRunningTask->isEmpty() && m_pRunningTask->contains(widget))
    {
        m_pRunningTask->removeOne(widget);
    }

    if (!m_pWaittingTask->isEmpty() && m_pWaittingTask->contains(widget))
    {
        m_pWaittingTask->removeOne(widget);
    }


    int index = m_pListWidget->findItem(item);

    if (index >= 0)
    {
        m_pListWidget->removeItemWidget(item);
        m_pListWidget->takeItem(index);

        delete widget;
        widget = nullptr;

        delete item;
        item = nullptr;
    }
}

void DownloadWidget::finishedTask(QListWidgetItem *item)
{
    DownloadList *widget = (DownloadList *)m_pListWidget->itemWidget(item);
    m_pRunningTask->removeOne(widget);

    if (m_pWaittingTask->count())
    {
        widget = (DownloadList *)m_pWaittingTask->takeFirst();
        widget->startDownload();
    }

    emit finished(widget->getFile());

    int index = m_pListWidget->findItem(item);

    if (index >= 0)
    {
        m_pListWidget->removeItemWidget(item);
        m_pListWidget->takeItem(index);

//        delete widget;
        widget = nullptr;

//        delete item;
        item = nullptr;
    }
}
