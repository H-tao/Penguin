#include "sftpserver.h"
#include "ui_sftpserver.h"

SftpServer::SftpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SftpServer)
{
    ui->setupUi(this);
}

SftpServer::SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber, QWidget *parent, TabPage *p)
    : QWidget(parent), ui(new Ui::SftpServer),
      m_connection(new QSsh::SshConnection(parameters)), serverNum(serverNumber), page(p),
      m_currentPath("/root/"), m_jobType(JobUnknow)
{
    ui->setupUi(this);

    initTreeView();

    connect(m_connection, SIGNAL(connected()), this, SLOT(handleConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), this, SLOT(handleError()));
    connect(m_connection, SIGNAL(disconnected()), this, SLOT(handleDisconnected()));


    m_connection->connectToHost();
}

SftpServer::~SftpServer()
{
    delete ui;
}

void SftpServer::handleConnected()
{
    qDebug() << "Create Channel";
    qDebug() << "Connection opened:" << m_connection->connectionParameters().host
             << ":" << m_connection->connectionParameters().port;

    // Create Sftp Channel and initialized
    m_channel = m_connection->createSftpChannel();

    connect(m_channel.data(), SIGNAL(initialized()),
            this, SLOT(handleChannelInitialized()));
    connect(m_channel.data(), SIGNAL(initializationFailed(QString)),
            this, SLOT(handleChannelInitializationFailed(QString)));
    connect(m_channel.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
            this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)), Qt::DirectConnection);
    connect(m_channel.data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            this, SLOT(handleFileInfo(QSsh::SftpJobId, QList<QSsh::SftpFileInfo>)));
    connect(m_channel.data(), SIGNAL(closed()), this, SLOT(handleChannelClosed()));

    m_channel->initialize();
}

void SftpServer::handleError()
{

}

void SftpServer::handleDisconnected()
{

}

void SftpServer::handleChannelInitialized()
{
    qDebug() << "Initialize channel success!";
    
    // list dir
    m_jobType = JobListDir;
    m_jobListDirId = m_channel->listDirectory(m_currentPath);
}

void SftpServer::handleChannelInitializationFailed(const QString &renson)
{

}

void SftpServer::handleJobFinished(QSsh::SftpJobId id, const QString &error)
{
    qDebug() << "handleJobFinished" << "error" << error;
    qDebug() << "finish : " << m_currentPath;
    if(!items.isEmpty())
    {
        QueueItem *i = items.front();
        m_currentItem = items.front()->it;
        m_currentPath = items.front()->strPath;
        items.pop_front();
        delete i;
        m_jobType = JobListDir;
        qDebug() << "currentItem : " << m_currentItem->text() << "currentPath : " << m_currentPath;
        m_jobListDirId = m_channel->listDirectory(m_currentPath);
    }
}

void SftpServer::handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList)
{
    qDebug() << "handleFileInfo";


    if(id != m_jobListDirId)
    {
        qDebug() << id;
        qDebug() << "id != m_jobListDirId";
        return;
    }

//    fileTree = new FileTreeView(this);
    if(fileInfoList.isEmpty())
    {
        return;
    }

    QString path = m_currentPath;
    foreach (const QSsh::SftpFileInfo &fi, fileInfoList)
    {
        qDebug()<<fi.name<<" "<<fi.size<<" "<<fi.type<<" "<<fi.permissions;

        if(fi.name == ".." || fi.name == ".")
        {
            continue;
        }

        if(m_currentPath == "/root/")
        {
            if(fi.type == QSsh::FileTypeRegular)
            {
                item = new QStandardItem(getFileIcon(fi.name), fi.name);
                m_treeItemModel->appendRow(item);
            }
            else if(fi.type == QSsh::FileTypeDirectory)
            {
                item = new QStandardItem(getFolderIcon(), fi.name);
                m_treeItemModel->appendRow(item);
//                getNextLevelList(m_currentPath + fi.name + "/");
                path = m_currentPath + fi.name + "/";
                QueueItem *tempQI = new QueueItem;
                tempQI->it = item;
                tempQI->strPath = path;
                items.append(tempQI);
            }
        }
        else
        {
            if(fi.type == QSsh::FileTypeRegular)
            {
                item = new QStandardItem(getFileIcon(fi.name), fi.name);
                m_currentItem->appendRow(item);
            }
            else if(fi.type == QSsh::FileTypeDirectory)
            {
                item = new QStandardItem(getFolderIcon(), fi.name);
                m_currentItem->appendRow(item);
//                getNextLevelList(m_currentPath + fi.name + "/");
                path = m_currentPath + fi.name + "/";
                QueueItem *tempQI = new QueueItem;
                tempQI->it = item;
                tempQI->strPath = path;
                items.append(tempQI);
            }
        }

    } // end foreash

//    m_currentItem = item;
//    m_currentPath = path;
}

void SftpServer::handleChannelClosed()
{

}

void SftpServer::getNextLevelList(QString path)
{
    qDebug() << "getNextLevelList : " << path;
    m_jobType = JobListDir;
    m_jobListDirId = m_channel->listDirectory(path);
//    m_currentPath = path;
//    qDebug() << "m_currentPath : " << m_currentPath;
}

void SftpServer::initTreeView()
{
    m_treeView =  new QFileTreeView(this);
    ui->layout->addWidget(m_treeView);

    m_treeView->setSortingEnabled(true);
    m_treeView->setAlternatingRowColors(true);
//    m_treeView->setRootIsDecorated(false);
//    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_treeItemModel = new QStandardItemModel(0, 1,this);
    m_treeItemModel->setHeaderData(0, Qt::Horizontal, tr("Name"));

    m_treeView->setModel(m_treeItemModel);
    m_treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
}
