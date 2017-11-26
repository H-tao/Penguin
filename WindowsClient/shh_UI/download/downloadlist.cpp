#include "downloadlist.h"

#include "../stable.h"

#include "../BasisCtrl/picturelabel.h"
#include "../BasisCtrl/pauselabel.h"
#include "../BasisCtrl/stoplabel.h"
#include "../tools/tools.h"
#include "function.h"
#include "../thread/downloadthread.h"

QMutex DownloadList::mutex;
DownloadList::DownloadList(QListWidget *listWidget, QWidget *parent, QSsh::SshConnection *connection) :
    QWidget(parent),
    m_pFile(nullptr),
    m_i64CurrentSize(0LL),
    m_pFileIco(nullptr),
    m_pFileName(nullptr),
    m_pFileSize(nullptr),
    m_pSurplusTime(nullptr),
    m_pProgressBar(nullptr),
    m_pCurrentSpeed(nullptr),
    m_pPauseButton(nullptr),
    m_pStopButton(nullptr),
    m_pDownloadThread(nullptr),
    m_connection(connection)
{
    setFixedHeight(50);
    initWidget();
    setWidgetLayout();

    m_eCurrentStatus = CurrentStatus::NOSTATUS;

    m_pListWidgetItem = new QListWidgetItem(listWidget);
    m_pListWidgetItem->setSizeHint(QSize(1, 50));
//    qDebug()<<"m_connection"<<m_connection;  
//    connect(channel.data(), SIGNAL(finish()), this, SLOT(recvDownloadFile_readyReadDownloadMsg()));
}

DownloadList::~DownloadList()
{
    m_pFile = 0;
    destroyWidget();
}


void DownloadList::setFile(QFile *file, QString localpath, QString remotepath, qint64 filesize)
{
    m_pFile = file;
    remotePath = remotepath;
    localPath = localpath;
    FileSize = filesize;

    QFileInfo fileInfo(remotePath+m_pFile->fileName());
    qDebug() << "remotePath::" << remotePath;
    //加载文件Ico

    setIco();
    setName(fileInfo.fileName());
    setSize(m_i64CurrentSize, FileSize);
    m_pSurplusTime->setText("--");
    updateProgressBar(0);
    setSpeed("等待中...");

    //加入就开始任务
    m_pPauseButton->startClick();
}

QFile *DownloadList::getFile() const
{
    return m_pFile;
}

QListWidgetItem *DownloadList::getListWidgetItem() const
{
    return m_pListWidgetItem;
}

void DownloadList::startDownload()
{
//    DownloadMsg downloadMsg;
//    memset(&downloadMsg, 0, sizeof(DownloadMsg));
//    strcpy(downloadMsg.fileName, m_pFile->getRemoteName().toUtf8().data());
    qDebug() << "DownloadList::startDownload";
    recvDownloadFile_readyReadDownloadMsg();
//    channel->downloadFile(remotePath + m_pFile->fileName(), localPath, QSsh::SftpOverwriteExisting);
//    m_pConnectToServer->sendDownloadMsg(downloadMsg);
}

void DownloadList::initWidget()
{
    m_pFileIco = new PictureLabel(this);
    m_pFileName = new QLabel(this);
    m_pFileSize = new QLabel(this);
    m_pSurplusTime = new QLabel(this);
    m_pProgressBar = new QProgressBar(this);
    m_pProgressBar->setMaximum(100);
    m_pCurrentSpeed = new QLabel(this);
    m_pPauseButton = new PauseLabel(this);
    m_pStopButton = new StopLabel(this);
    connect(m_pPauseButton, SIGNAL(start()), this, SLOT(start_pauseButton()));
    connect(m_pPauseButton, SIGNAL(pause()), this, SLOT(pause_pauseButton()));
    connect(m_pStopButton, SIGNAL(stop()), this, SLOT(stop_stopButton()));
}

void DownloadList::setWidgetLayout()
{
    int height = this->height();
    int width = this->width();

    //二分之一
    int height_2 = height / 2;
    int width_2 = width / 2;

    m_pFileIco->move(5, height_2 - 20);
    m_pFileIco->resize(QSize(35, 40));

    m_pFileName->resize(QSize(width_2 - 45, 20));
    m_pFileName->move(45, height_2 - 20);
    QFontMetrics elideFont(m_pFileName->font());
    m_pFileName->setText(elideFont.elidedText(m_pFileName->text(), Qt::ElideRight, m_pFileName->width()));

    m_pFileSize->resize(QSize(width_2 - 45, 20));
    m_pFileSize->move(45, height_2);

    m_pSurplusTime->move(width_2, height_2 - 10);
    m_pSurplusTime->resize(QSize(55, 20));

    m_pStopButton->move(width - 45, height_2 - 10);

    m_pPauseButton->move(width - 100, height_2 - 10);

    m_pProgressBar->move(width_2 + 80, height_2 - 20);
    m_pProgressBar->resize(width_2 - 200, 20);

    m_pCurrentSpeed->move(width_2 + 80, height_2);
    m_pCurrentSpeed->resize(width_2 - 200, 20);
}

void DownloadList::destroyWidget()
{
    if(m_pFileIco)
        delete m_pFileIco;
    m_pFileIco = nullptr;

    if(m_pFileName)
        delete m_pFileName;
    m_pFileName = nullptr;

    if(m_pFileSize)
        delete m_pFileSize;
    m_pFileSize = nullptr;

    if(m_pSurplusTime)
        delete m_pSurplusTime;
    m_pSurplusTime = nullptr;

    if(m_pProgressBar)
        delete m_pProgressBar;
    m_pProgressBar = nullptr;

    if(m_pCurrentSpeed)
        delete m_pCurrentSpeed;
    m_pCurrentSpeed = nullptr;

    if(m_pPauseButton)
        delete m_pPauseButton;
    m_pPauseButton = nullptr;

    if(m_pStopButton)
        delete m_pStopButton;
    m_pStopButton = nullptr;
}

void DownloadList::setIco()
{

}

void DownloadList::setName(QString name)
{
    //设置文件名字
    //如果文件名太长，显示...
//    QFontMetrics elideFont(m_pFileName->font());
//    m_pFileName->setText(elideFont.elidedText(name, Qt::ElideRight, m_pFileName->width()));
     m_pFileName->setText(name);
}

void DownloadList::setSize(qint64 currentSize, qint64 totalSize)
{
    QString cSize = Tools::sizeToString(currentSize);
    QString tSize = Tools::sizeToString(totalSize);
    qDebug() << "cSize1::" << cSize;
    qDebug() << "tSize2::" << tSize;
    m_pFileSize->setText(cSize + QString("/") + tSize);
}

void DownloadList::updateProgressBar(int progress)
{
    m_pProgressBar->setValue(progress);
}

void DownloadList::setSpeed(QString speed)
{
    m_pCurrentSpeed->setText(speed);
}

void DownloadList::setSpeed(qint64 speed)
{
    QString speedStr = Tools::sizeToString(speed);
    setSpeed(speedStr + QString("/s"));
}

void DownloadList::resizeEvent(QResizeEvent *e)
{
    setWidgetLayout();
    QWidget::resizeEvent(e);
}

void DownloadList::timerEvent(QTimerEvent *event)
{
//    qint64 currsize = QFileInfo(localPath).size();
//    updateTask_currentSize(currsize);
}

void DownloadList::updateTask_currentSize(qint64 currentSize)
{
    qDebug() << "DownloadList::updateTask_currentSize";
    qDebug() << "currentSize : " << currentSize;
//    currentSize = QFileInfo(localPath).size();
    setSpeed(currentSize - m_i64CurrentSize);
    m_i64CurrentSize = currentSize;
    setSize(m_i64CurrentSize, FileSize);
    if(m_i64CurrentSize * 1.0 / FileSize * 100 > 99.9)
        updateProgressBar(100);
    else
        updateProgressBar(m_i64CurrentSize * 1.0 / FileSize * 100);

    if(m_i64CurrentSize == FileSize)
    {
        if(m_eCurrentStatus == CurrentStatus::RUNNING)
        {
            delete m_pDownloadThread;
            m_pDownloadThread = nullptr;
        }

        m_eCurrentStatus = CurrentStatus::FINISHED;
        emit finished(m_pListWidgetItem);
    }
}

void DownloadList::start_pauseButton()
{
    m_eCurrentStatus = CurrentStatus::WAITTING;
    emit start(m_pListWidgetItem);
}

void DownloadList::pause_pauseButton()
{
    if(m_eCurrentStatus == CurrentStatus::RUNNING)
    {
        m_pDownloadThread->pause();
        delete m_pDownloadThread;
        m_pDownloadThread = nullptr;
    }

    m_eCurrentStatus = CurrentStatus::PAUSED;
    emit pause(m_pListWidgetItem);
}

void DownloadList::stop_stopButton()
{
    if(m_eCurrentStatus == CurrentStatus::RUNNING)
    {
        m_pDownloadThread->stop();
        delete m_pDownloadThread;
        m_pDownloadThread = nullptr;
    }

    m_eCurrentStatus = CurrentStatus::STOP;
    emit stop(m_pListWidgetItem);
}

void DownloadList::recvDownloadFile_readyReadDownloadMsg()
{
//    disconnect(m_pConnectToServer, SIGNAL(readyReadDownloadMsg(DownloadMsg)), this, SLOT(recvDownloadFile_readyReadDownloadMsg(DownloadMsg)));

    if (m_eCurrentStatus == CurrentStatus::WAITTING)
    {
        m_eCurrentStatus = CurrentStatus::RUNNING;

        m_pDownloadThread = new DownloadThread(m_connection,localPath, remotePath+m_pFile->fileName(), this);
        connect(m_pDownloadThread, &DownloadThread::currentTaskProgress, this, &DownloadList::updateTask_currentSize);

        m_pDownloadThread->start();
        ThreadPool::getInstance()->addJob(m_pDownloadThread);
    }
}

