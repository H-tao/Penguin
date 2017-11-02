#include "sftpserver.h"
#include "ui_sftpserver.h"
#include <QFileDialog>
#include <QTimer>
#include <QThread>

QString homePath = "/";
bool isFirstLink = true;

SftpServer::SftpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SftpServer)
{
    ui->setupUi(this);
}

SftpServer::SftpServer(const QSsh::SshConnectionParameters &parameters, int serverNumber, QWidget *parent, TabPage *p, MainWindow *mainWindow)
    : QWidget(parent), ui(new Ui::SftpServer),
      m_connection(new QSsh::SshConnection(parameters)), serverNum(serverNumber), page(p),
      m_currentPath(homePath), m_shellPath(homePath), m_jobType(JobUnknow)
{
    ui->setupUi(this);
    m_mainWindow = reinterpret_cast<MainWindow*>(mainWindow);

    initTreeView();
    initPage();
    initProgressDialog();

    connect(m_connection, SIGNAL(connected()), this, SLOT(handleConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), this, SLOT(handleError()));
    connect(m_connection, SIGNAL(disconnected()), this, SLOT(handleDisconnected()));

    // 设置Geometry
//    this->setGeometry(m_mainWindow->x()-this->width(), m_mainWindow->y(), this->width(), m_mainWindow->height());
    this->resize(this->width(), m_mainWindow->height());
    this->move(m_mainWindow->x()-this->width(), m_mainWindow->y());
    m_connection->connectToHost();

//    page->openFileSystem();
}

SftpServer::~SftpServer()
{
    delete ui;
}

Channel_Type* SftpServer::createNewChannel(Channel_Type * channel)
{
    //qDebug() << "createNewChannel";
//    Channel_Type* channel = new Channel_Type;
//    channel = m_connection->createSftpChannel();
//    channels.append(channel);

//    connect(channel->data(), SIGNAL(initialized()),
//            this, SLOT(handleChannelInitialized()));
    connect(channel->data(), SIGNAL(initializationFailed(QString)),
            this, SLOT(handleChannelInitializationFailed(QString)));
    connect(channel->data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
            this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)), Qt::DirectConnection);
    connect(channel->data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            this, SLOT(handleFileInfo(QSsh::SftpJobId, QList<QSsh::SftpFileInfo>)));
    connect(channel->data(), SIGNAL(closed()), this, SLOT(handleChannelClosed()));

    return channel;
}

void SftpServer::handleConnected()
{
    qDebug() << "SftpServer::handleConnected";
    //qDebug() << "Create Channel";
    //qDebug() << "Connection opened:" << m_connection->connectionParameters().host
             //<< ":" << m_connection->connectionParameters().port;

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

    channel_2 = m_connection->createSftpChannel();

    connect(channel_2.data(), SIGNAL(initialized()),
            this, SLOT(handleChannelInitialized()));
    connect(channel_2.data(), SIGNAL(initializationFailed(QString)),
            this, SLOT(handleChannelInitializationFailed(QString)));
    connect(channel_2.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
           this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)), Qt::DirectConnection);
    connect(channel_2.data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
           this, SLOT(handleFileInfo(QSsh::SftpJobId, QList<QSsh::SftpFileInfo>)));
    connect(channel_2.data(), SIGNAL(closed()), this, SLOT(handleChannelClosed()));

    channel_2->initialize();
}

void SftpServer::handleError()
{

}

void SftpServer::handleDisconnected()
{

}

void SftpServer::handleChannelInitialized()
{
    //qDebug() << "Initialize channel success!";

    if(isFirstLink)
    {
        isFirstLink = false;
        // list dir
        m_jobType = JobListDir;
        m_workWidget = WorkFileTreeView;
        m_jobListDirId = m_channel->listDirectory(m_currentPath);
    }
}

void SftpServer::handleChannelInitializationFailed(const QString &reason)
{
    //qDebug() << "handleChannelInitializationFailed";
    QMessageBox::warning(page->fileWidget, tr("CreateNewChannelFailed"), tr("reason") + reason, QMessageBox::Ok);
}

void SftpServer::handleJobFinished(QSsh::SftpJobId id, const QString &error)
{
    qDebug() << "handleJobFinished";
    if(!error.isEmpty())
        //qDebug() << "error" << error;
    //qDebug() << "finished currentPath: " << m_currentPath;
    m_currentPath = m_shellPath;
    //qDebug() << "currentPaht = shellPath" << m_currentPath;
    //qDebug() << "finished shellPath: " << m_shellPath;

    if(!error.isEmpty())
    {
        m_jobType = JobUnknow;
        killTimer(m_timer);
        m_progress->cancel();
    }


    if(m_workWidget == WorkFileTreeView)
    {
        if(!items.isEmpty())
        {
            QueueItem *i = items.front();
            m_currentItem = items.front()->it;
            m_currentPath = items.front()->strPath;
            items.pop_front();
            delete i;
            m_jobType = JobListDir;
            //qDebug() << "currentItem : " << m_currentItem->text() << "currentPath : " << m_currentPath;
            m_jobListDirId = m_channel->listDirectory(m_currentPath);
        }
    }
    if(m_workWidget == WorkFileWidget)
    {
        switch(m_jobType)
        {
        case JobListDir:
//            page->fileWidget->refreshDirectory(m_fileInfoList);
            break;
        case JobCreateDir:
            // refresh directory
            handleRefreshClicked();
            break;
        case JobCreateFile:
            // refresh directory
            handleRefreshClicked();
            break;
        case JobRemoveDir:
            // refresh directory
            handleRefreshClicked();
            break;
        case JobRemoveFile:
            // refresh directory
            handleRefreshClicked();
            break;
        case JobRename:
            // refresh directory
            handleRefreshClicked();
            break;
        case JobDownloadFile:
            killTimer(m_timer);
            m_progress->setValue(100);
            QTest::qSleep(1000);
            m_progress->cancel();
            break;
        case JobUploadFile:
            killTimer(m_timer);
            m_progress->setValue(100);
            QTest::qSleep(1000);
            m_progress->cancel();
            // refresh directory
            handleRefreshClicked();
            break;
        default:
            m_jobType = JobUnknow;
            break;
        }
        return;
    }
}

void SftpServer::handleFileInfo(QSsh::SftpJobId id, const QList<QSsh::SftpFileInfo> &fileInfoList)
{
    //qDebug() << "handleFileInfo";


    if(id != m_jobListDirId)
    {
        //qDebug() << id;
        //qDebug() << "id != m_jobListDirId";
        return;
    }

    if(fileInfoList.isEmpty())
    {
        return;
    }

    /************* FileTreeView ******************/
    if(m_workWidget == WorkFileTreeView)
    {
        QString path = m_currentPath;
        foreach (const QSsh::SftpFileInfo &fi, fileInfoList)
        {
            //qDebug()<<fi.name<<" "<<fi.size<<" "<<fi.type<<" "<<fi.permissions;

            if(fi.name == ".." || fi.name == ".")
            {
                continue;
            }

            if(m_currentPath == homePath)
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
                    path = m_currentPath + fi.name + "/";
                    QueueItem *tempQI = new QueueItem;
                    tempQI->it = item;
                    tempQI->strPath = path;
                    items.append(tempQI);
                }
            }// end if
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
                    path = m_currentPath + fi.name + "/";
                    QueueItem *tempQI = new QueueItem;
                    tempQI->it = item;
                    tempQI->strPath = path;
                    items.append(tempQI);
                }
            }// end else
        } // end foreash

        m_treeView->sortByColumn(m_treeView->header()->sortIndicatorSection(),
                                  m_treeView->header()->sortIndicatorOrder());
    }// end WorkFileTreeView

    /************* FileWidget ******************/
    if(m_workWidget == WorkFileWidget)
    {
        page->fileWidget->refreshDirectory(fileInfoList);
//        m_fileInfoList.append(fileInfoList);
        page->filePathLineEdit->setCurrentText(m_shellPath);
    }


}

void SftpServer::handleChannelClosed()
{

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

void SftpServer::initPage()
{
    // 初始化文件系统的子部件
//    page->initStyle();
    connect(page->fileWidget, SIGNAL(openFileSystemClicked()), this, SLOT(handleOpenFileWidgetClicked()));
    connect(page->fileWidget, SIGNAL(openClicked(QString,QString,QString)),
            this, SLOT(handleOpenClicked(QString,QString,QString)));
    connect(page->fileWidget, SIGNAL(upClicked()), this, SLOT(handleUpClicked()));
    connect(page->fileWidget, SIGNAL(homeClicked()), this, SLOT(handleHomeClicked()));
    connect(page->fileWidget, SIGNAL(downloadClicked(QString,QString,QString)),
            this, SLOT(handleDownloadClicked(QString,QString,QString)));
    connect(page->fileWidget, SIGNAL(refreshClicked()), this, SLOT(handleRefreshClicked()));
    connect(page->fileWidget, SIGNAL(deleteClicked(QString,QString)), this, SLOT(handleDeleteClicked(QString,QString)));
    connect(page->fileWidget, SIGNAL(uploadClicked()), this, SLOT(handleUploadClicked()));
    connect(page->fileWidget, SIGNAL(renameClicked(QString)), this, SLOT(handleRenameClicked(QString)));
    connect(page->fileWidget, SIGNAL(newFolderClicked()), this, SLOT(handleNewFolderClicked()));
    connect(page->fileWidget, SIGNAL(newFileClicked()), this, SLOT(handleNewFileClicked()));

    connect(page->maxiBtn, SIGNAL(clicked(bool)), page, SLOT(openFileSystem()));
    connect(page->miniBtn, SIGNAL(clicked(bool)), page, SLOT(concealFileSystem()));
    connect(page->filePathLineEdit, SIGNAL(activated(QString)), this, SLOT(lineEditChanged(QString)));
    connect(page->searchEdit, SIGNAL(activated(QString)), this, SLOT(searchEditChanged(QString)));
    page->filePathLineEdit->setCurrentText(m_shellPath);
    page->searchEdit->lineEdit()->setPlaceholderText("此目录\”搜索\"");

    m_fileWidgetModel = page->fileWidget->m_model;
}

void SftpServer::initProgressDialog()
{
    //qDebug() << "initProgressDialog";

    m_progress = new QProgressDialog(page->fileWidget, Qt::Dialog | Qt::CustomizeWindowHint);
    m_progress->setWindowModality(Qt::WindowModal);
    m_progress->setMaximumWidth(800);
    m_progress->setCancelButton(0);
    m_progress->setAutoClose(true);
    m_progress->setAutoReset(false);

    m_progress->setRange(0, 100);
    m_progress->cancel();
}

void SftpServer::handleOpenFileWidgetClicked()
{
    qDebug() << "handleOpenFileWidgetClicked()";

    //list
    handleRefreshClicked();
}

void SftpServer::handleOpenClicked(const QString &fileName,const QString &fileType, const QString &fileSize)
{
    //qDebug() << "handleOpenClicked";

    if(fileType == getFolderType())
    {
        m_shellPath = m_shellPath + fileName + "/";
        //qDebug() << "Current Shell Path : " << m_shellPath;

        // list directory
        handleRefreshClicked();
    }
    else
    {
        //qDebug() << "Not a file, can't open, choose to download!";
        if(QMessageBox::Yes ==
                QMessageBox::question(page->fileWidget, tr("open"),
                                      tr("This file is not a folder, do you want to download?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
        {
            handleDownloadClicked(fileName, fileType, fileSize);
        }
        else
        {
//            page->fileWidget->currentItem()->setSelected(true);
        }
        return;
    }
}

void SftpServer::handleUpClicked()
{
    //qDebug() << "handleUpClicked";

    if(m_shellPath == homePath)
    {
        //qDebug() << "m_shellPath == homePath, is home , can't Up!";
        return;
    }

    QString path = m_shellPath;

    // chop the end
    path.chop(1);

    // if the root is "/", after chop the end, the path will be empty
    if(path.isEmpty())
    {
        return;
    }

    QString parentPath = path.left(path.lastIndexOf("/") + 1);

    // update the shellPath
    m_shellPath = parentPath;
    //qDebug() << "m_shellPath : " << m_shellPath;

    // list up direcotry
    handleRefreshClicked();
}

void SftpServer::handleHomeClicked()
{
    //qDebug() << "handleHomeClicked";

    // update shellPath to homePath
    m_shellPath = homePath;

    // list up direcotry
    handleRefreshClicked();
}

void SftpServer::handleDownloadClicked(const QString &fileName, const QString &fileType,const QString &fileSize)
{
    //qDebug() << "handleDownloadClicked";
    m_currentSize = getSizeToByte(fileSize);

    if(fileType == getFolderType())
    {
        QMessageBox::warning(this, tr("Download"), tr("Can not download folder!"),
                             QMessageBox::Ok);
        return;
    }

    m_currentLocalFilePath = QFileDialog::getSaveFileName(page->fileWidget, tr("Download File"),
                                                      QDir::currentPath() + "/" + fileName, tr("All Files (*.*"));

    if(m_currentLocalFilePath.isEmpty())
        return;
    //qDebug() << "Local File : " << m_currentLocalFilePath;

    // download file
    m_jobType = JobDownloadFile;
    m_workWidget = WorkFileWidget;
    m_channel->downloadFile(m_shellPath + fileName, m_currentLocalFilePath, QSsh::SftpOverwriteExisting);

    m_progress->setLabelText(tr("Download... ") + fileName +
                             tr(" to ") + QFileInfo(m_currentLocalFilePath).absolutePath());

    m_progress->reset();
    m_progress->show();
    m_progress->setValue(0);
    m_timer = startTimer(500);
}

void SftpServer::handleRefreshClicked()
{
    qDebug() << "handleRefreshClicked";

    m_jobType = JobListDir;
    m_workWidget = WorkFileWidget;
    m_fileWidgetModel->removeRows(0, page->fileWidget->m_model->rowCount(QModelIndex()));
    m_jobListDirId = m_channel->listDirectory(m_shellPath);
}

void SftpServer::handleDeleteClicked(const QString &fileName, const QString &fileType)
{
    //qDebug() << "handleDeleteClicked";

    QString filePath = m_shellPath + fileName;
    if(fileType == getFolderType())
    {
        //qDebug() << "Delete Dir: " << (filePath + "/");
        //qDebug() << "内有文件删除不了，暂未处理（可通过shell处理）";
        m_channel->removeDirectory(filePath + "/");

    }
    else
    {
        m_channel->removeFile(filePath);
    }

    // refresh direcotry
    handleRefreshClicked();
}

void SftpServer::handleUploadClicked()
{
    //qDebug() << "handleUploadClicked";

    QString localPath = QFileDialog::getOpenFileName(page->fileWidget, tr("Upload File"),
                                                     QDir::currentPath(), tr("All File (*.*)"));

    //qDebug() << "Upload local file : " << localPath;
    upload(localPath);
}

void SftpServer::upload(QString localPath)
{
    if(QFileInfo(localPath).isSymLink())
    {
        localPath = QFileInfo(localPath).symLinkTarget();
    }

    m_currentLocalFilePath = localPath;
    QString fileName = QFileInfo(localPath).fileName();
    m_currentSize = QFileInfo(localPath).size();

    if(m_currentLocalFilePath.isEmpty())
    {
        return;
    }

    // Jude if the same name have existed
    if(page->fileWidget->isFileExisted(m_currentLocalFilePath))
    {
        if(QMessageBox::No == QMessageBox::warning(page->fileWidget, tr("Upload"),
                                                   tr("This file has existed, are you sure to replace it?"),
                                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes
                                                   ))
        {
            return;
        }
    }

    // start upload
    m_currentRemoteFilePath = m_shellPath + fileName;
    m_workWidget = WorkFileWidget;
    m_jobType = JobUploadFile;
//    QThread::msleep(2000);
 //   m_channel->uploadFile(m_currentLocalFilePath, m_currentRemoteFilePath, QSsh::SftpOverwriteExisting);
    channel_2->uploadFile(m_currentLocalFilePath, m_currentRemoteFilePath, QSsh::SftpOverwriteExisting);

    // show progress
    m_progress->setLabelText(tr("Upload... ") + m_currentLocalFilePath +
                             tr(" to ") + m_currentRemoteFilePath);
    m_progress->reset();
    m_progress->show();
    m_progress->setValue(0);

    m_timer = startTimer(500);
}

void SftpServer::handleRenameClicked(const QString &fileName)
{
    //qDebug() << "handleRenameClicked";

    FileNameDialog dialog(page->fileWidget);
    dialog.setWindowTitle("Rename");
    dialog.setText(fileName);

    if(QDialog::Rejected == dialog.exec())
    {
        return;
    }

    // Jude if the same name have existed
    if(page->fileWidget->isFileExisted(dialog.getText()))
    {
        QMessageBox::critical(page->fileWidget, tr("error"),
                              tr("This file has existed in current path"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_selectName = dialog.getText();
    m_workWidget = WorkFileWidget;
    m_jobType = JobRename;
    m_channel->renameFileOrDirectory(m_shellPath + fileName, m_shellPath + m_selectName);
    //qDebug() << "m_channel " << m_channel;
}

void SftpServer::handleNewFolderClicked()
{
    //qDebug() << "handleNewFolderClicked";

    FileNameDialog dialog(page->fileWidget);
    dialog.setWindowTitle("New Folder");
    dialog.setText("new folder");

    if(QDialog::Rejected == dialog.exec())
    {
        return;
    }

    /************ 注意：存在同名文件或文件夹均不能创建成功 ***************/
    // Jude if the same name have existed
    if(page->fileWidget->isFolderExisted(dialog.getText()))
    {
        QMessageBox::critical(page->fileWidget, tr("error"),
                              tr("This folder has existed in current path"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_selectName = dialog.getText();
    m_workWidget = WorkFileWidget;
    m_jobType = JobCreateDir;
    m_channel->createDirectory(m_shellPath + m_selectName);
}


void SftpServer::handleNewFileClicked()
{
    //qDebug() << "handleNewFileClicked";

    FileNameDialog dialog(page->fileWidget);
    dialog.setWindowTitle("New File");
    dialog.setText("new file");

    if(QDialog::Rejected == dialog.exec())
    {
        return;
    }

    /************ 注意：存在同名文件或文件夹均不能创建成功 ***************/
    // Jude if the same name have existed
    if(page->fileWidget->isFileExisted(dialog.getText()))
    {
        QMessageBox::critical(page->fileWidget, tr("error"),
                              tr("This name has existed in current path"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_selectName = dialog.getText();
    m_workWidget = WorkFileWidget;
    m_jobType = JobCreateFile;
    m_channel->createFile(m_shellPath + m_selectName, QSsh::SftpOverwriteExisting);
}

void SftpServer::openTreeView()
{
    this->show();
}

void SftpServer::lineEditChanged(QString pathChanged)
{
    //qDebug() << "lineEditChanged";

    int i = 0;
    int j = -1;
    while(i < pathChanged.length())
    {
        if(pathChanged.at(i) == '/')
        {
            if(j + 1 == i)
            {
                pathChanged.remove(i,1);
                continue;
            }
            j = i;
        }
        ++i;
    }

    // add '/' at the front
    pathChanged.push_front('/');

    // add '/' at the back
    if(pathChanged.right(1) != "/")
    {
        pathChanged.push_back('/');
    }

    //qDebug() << "Path Changed : " << pathChanged;

    m_shellPath = pathChanged;

    handleRefreshClicked();
}

void SftpServer::searchEditChanged(QString fileName)
{
    QList<QStandardItem*> target = m_fileWidgetModel->findItems(fileName, Qt::MatchExactly);
    if(target.size() == 0)
    {
        QMessageBox::warning(page->fileWidget, tr("提示"),
                             tr("Can't find '%1' in this directory!").arg(fileName), QMessageBox::Ok);
        page->searchEdit->lineEdit()->selectedText();
    }
    if(target.size() >= 1)
    {
        QStandardItem *item = target.at(0);
        page->fileWidget->setCurrentIndex(item->index());
    }
}
