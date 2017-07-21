#include "sftpserver.h"
#include "ui_sftpserver.h"

SftpServer::SftpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SftpServer)
{
    ui->setupUi(this);
}

SftpServer::SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber, QWidget *parent)
    : QWidget(parent),
      m_connection(new QSsh::SshConnection(parameters)), serverNum(serverNumber), 
      m_currentPath("/"), m_connection(0), m_jobType(JobUnknow)
{
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
    connect(m_channel.data(), SIGNAL(initializedFailed()),
            this, SLOT(handleChannelInitializedFailed()));
    connect(m_channel.data(), SIGNAL(finished()),
            this, SLOT(handleJobFinished()));
    connect(m_channel.data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            this, SLOT(handleFileInfo()));
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
    qDebug << "Initialize channel success!";
    
    // list dir
    m_jobType = JobListDir;
    m_jobListDirId = m_channel->listDirectory(m_currentPath);
}

void SftpServer::handleChannelInitializedFailed()
{

}

void SftpServer::handleJobFinished()
{

}

void SftpServer::handleFileInfo()
{

}

void SftpServer::handleChannelClosed()
{

}
