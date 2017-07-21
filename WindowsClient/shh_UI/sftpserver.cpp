#include "sftpserver.h"
#include "ui_sftpserver.h"

SftpServer::SftpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SftpServer)
{
    ui->setupUi(this);
}

SftpServer::SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber, QWidget *parent)
    : QWidget(parent), ui(new Ui::SftpServer),
      m_connection(new QSsh::SshConnection(parameters)), serverNum(serverNumber),
      m_currentPath("/"), m_jobType(JobUnknow)
{
    ui->setupUi(this);

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
            this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)));
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
    qDebug() << "handleJobFinished";
}

void SftpServer::handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList)
{
    qDebug() << "handleFileInfo";

    fileTree = new FileTreeView(this);
    foreach (const QSsh::SftpFileInfo &fi, fileInfoList)
    {
        qDebug()<<fi.name<<" "<<fi.size<<" "<<fi.type<<" "<<fi.permissions;
        if(fi.name == ".." || fi.name == ".")
        {
            continue;
        }
        if(fi.type == QSsh::FileTypeRegular)
            fileTree->addTreeRoot(fi.name, getFileIcon(fi.name));
        if(fi.type == QSsh::FileTypeDirectory)
            fileTree->addTreeRoot(fi.name, getFolderIcon());


    }
    ui->layout->addWidget(fileTree);
}

void SftpServer::handleChannelClosed()
{

}
