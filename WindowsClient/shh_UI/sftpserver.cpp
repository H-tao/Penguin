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

    connect(this,SIGNAL(openFileName(QString,QString,qint64)),this,SLOT(openTextEdit(QString,QString,qint64)));
}

SftpServer::~SftpServer()
{
    delete ui;
}

void SftpServer::createNewChannel(Channel_Type &channel)
{
    //qDebug() << "createNewChannel";
    channel = m_connection->createSftpChannel();

    connect(channel.data(), SIGNAL(initialized()),
            this, SLOT(handleChannelInitialized()), Qt::DirectConnection);
    connect(channel.data(), SIGNAL(initializationFailed(QString)),
            this, SLOT(handleChannelInitializationFailed(QString)));
    connect(channel.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
            this, SLOT(handleJobFinished(QSsh::SftpJobId, QString)), Qt::DirectConnection);
    connect(channel.data(), SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            this, SLOT(handleFileInfo(QSsh::SftpJobId, QList<QSsh::SftpFileInfo>)));
    connect(channel.data(), SIGNAL(closed()), this, SLOT(handleChannelClosed()));

    channel->initialize();
}

void SftpServer::handleConnected()
{
    //qDebug() << "SftpServer::handleConnected";
    //qDebug() << "Create Channel";
    //qDebug() << "Connection opened:" << m_connection->connectionParameters().host
             //<< ":" << m_connection->connectionParameters().port;

    // Create Sftp Channel and initialized
    downloadWidge = new DownloadWidget(NULL,m_connection);
    page->multiDownloadLayout->addWidget(downloadWidge);
//    downloadWidge->show();

    this->createNewChannel(m_channel);
    this->createNewChannel(channel_2);
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

    if(isFirstLink)
    {
        qDebug() << "isFirstLink";
        isFirstLink = false;
        // list dir
        m_jobType = JobListDir;
        m_workWidget = WorkFileTreeView;
        m_jobListDirId = m_channel->listDirectory(m_currentPath);
        handleOpenFileWidgetClicked();
     //   emit connSuccess();
    }
}

void SftpServer::handleChannelInitializationFailed(const QString &reason)
{
    //qDebug() << "handleChannelInitializationFailed";
    QMessageBox::warning(page->fileWidget, tr("CreateNewChannelFailed"), tr("reason") + reason, QMessageBox::Ok);
}

void SftpServer::handleJobFinished(QSsh::SftpJobId id, const QString &error)
{
   // qDebug() << "handleJobFinished";

    if(!error.isEmpty())
    {
        m_jobType = JobUnknow;
        killTimer(m_timer);
        m_progress->cancel();

        m_shellPath = m_beforePath;
        m_mainWindow->statusBar()->showMessage(tr("error: ") + error);
        page->filePathLineEdit->setEditText(m_shellPath);

        return;
    }

    m_mainWindow->statusBar()->showMessage(tr("operation success!"));
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
            if(isOpenFile)
            {
                //qDebug() << "emit openFileName(commingFileName, commingFileType, commingFileSize)";
                emit openFileName(commingFileName, commingFileType, commingFileSize);
                isOpenFile = false;
                break;
            }
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
   // qDebug() << "handleFileInfo";

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

    // 通过timeEvent获得上传文件的信息，更新进度条
    if((m_jobType == JobUploadFile) && (id == m_jobUploadId))
    {
        int progress = 100*(fileInfoList[0].size / m_currentSize);
        m_progress->setValue(progress);
        if(progress >= 100)
        {
            killTimer(m_timer);
            //暂停一秒
            QTest::qSleep(1000);
            m_progress->cancel();

            m_selectName = fileInfoList[0].name;
            m_jobType = JobListDir;
            m_jobListDirId = m_channel->listDirectory(m_shellPath);

        }
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
        m_fileWidgetModel->removeRows(0, page->fileWidget->m_model->rowCount(QModelIndex()));
        page->fileWidget->refreshDirectory(fileInfoList);
        page->filePathLineEdit->setEditText(m_shellPath);
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
    connect(page->fileWidget, SIGNAL(uploadClicked(QString)), this, SLOT(handleUploadClicked(QString)));
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
    //qDebug() << "handleOpenFileWidgetClicked()";

    //list
    handleRefreshClicked();
}

void SftpServer::handleOpenClicked(const QString &fileName,const QString &fileType, const QString &fileSize)
{
    //qDebug() << "handleOpenClicked";

    if(fileType == getFolderType())
    {
        m_beforePath = m_shellPath;     //记录改变前的路径
        m_shellPath = m_shellPath + fileName + "/";
        //qDebug() << "Current Shell Path : " << m_shellPath;

        // list directory
        handleRefreshClicked();
    }
    else
    {
        m_jobType = JobDownloadFile;
        m_workWidget = WorkFileWidget;
        m_channel->downloadFile(m_shellPath + fileName, QDir::currentPath() +"/"+ fileName, QSsh::SftpOverwriteExisting);
        openningFileName.append(m_shellPath+fileName);
        commingFileName = QDir::currentPath()+"/" + fileName;
        commingFileSize = getSizeToByte(fileSize);
        commingFileType = fileType;
        isOpenFile = true;
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

    m_beforePath = m_shellPath;     //记录改变前的路径
    // update the shellPath
    m_shellPath = parentPath;
    //qDebug() << "m_shellPath : " << m_shellPath;

    // list up direcotry
    handleRefreshClicked();
}

void SftpServer::handleHomeClicked()
{
    //qDebug() << "handleHomeClicked";

    m_beforePath = m_shellPath;     //记录改变前的路径
    // update shellPath to homePath
    m_shellPath = homePath;

    // list up direcotry
    handleRefreshClicked();
}

void SftpServer::handleDownloadClicked(const QString &fileName, const QString &fileType,const QString &fileSize)
{
    //qDebug() << "handleDownloadClicked";
    m_currentSize = getSizeToByte(fileSize);
    QFile *file = new QFile(fileName);

    if(fileType == getFolderType())
    {
        QMessageBox::warning(this, tr("Download"), tr("Can not download folder!"),
                             QMessageBox::Ok);
        return;
    }

    m_currentLocalFilePath = QFileDialog::getSaveFileName(page->fileWidget, tr("Download File"),
                                                      QDir::currentPath() + "/" + fileName, tr("All Files (*"));

    //qDebug() << "Local File : " << m_currentLocalFilePath;
    if(m_currentLocalFilePath.isEmpty())
        return;

    // download file
    m_jobType = JobDownloadFile;
    m_workWidget = WorkFileWidget;
    downloadWidge->addTask(m_currentLocalFilePath, file, m_shellPath, m_currentSize);
//    m_channel->downloadFile(m_shellPath + fileName, m_currentLocalFilePath, QSsh::SftpOverwriteExisting);

//    m_progress->setLabelText(tr("Download... ") + fileName +
//                             tr(" to ") + QFileInfo(m_currentLocalFilePath).absolutePath());

//    m_progress->reset();
//    m_progress->show();
//    m_progress->setValue(0);
//    m_timer = startTimer(500);
}

void SftpServer::handleRefreshClicked()
{
    //qDebug() << "handleRefreshClicked";

    m_jobType = JobListDir;
    m_workWidget = WorkFileWidget;
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

void SftpServer::handleUploadClicked(QString localPath)
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
    if(page->fileWidget->isFileExisted(fileName))
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
    QString fileName = dialog.getText();

    /************ 注意：存在同名文件或文件夹均不能创建成功 ***************/
    // Jude if the same name have existed
    if(page->fileWidget->isFileExisted(fileName))
    {
        QMessageBox::critical(page->fileWidget, tr("error"),
                              tr("This folder has existed in current path"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_selectName = fileName;
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

    QString fileName = dialog.getText();
    /************ 注意：存在同名文件或文件夹均不能创建成功 ***************/
    // Jude if the same name have existed
    if(page->fileWidget->isFileExisted(fileName))
    {
        QMessageBox::critical(page->fileWidget, tr("error"),
                              tr("This name has existed in current path"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_selectName = fileName;
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

    m_beforePath = m_shellPath;     //记录改变前的路径
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
    }
    if(target.size() >= 1)
    {
        QStandardItem *item = target.at(0);
        page->fileWidget->setCurrentIndex(item->index());
    }
}

void SftpServer::openTextEdit(QString FileName, QString FileType, qint64 FileSize)
{
    TextEdit *p;
    p= new TextEdit(this,openningFileName.size()-1);
    QRect deskRect = QApplication::desktop()->availableGeometry();
    p->move((QApplication::desktop()->width() - p->width())/2,(QApplication::desktop()->height() - p->height())/2);
    p->show();
    p->run(FileName,FileType);
    connect(p,SIGNAL(SaveOpenFile(QString,QString,int)),this,SLOT(SaveOpenFile(QString,QString,int)));
}

void SftpServer::SaveOpenFile(QString file,QString type, int number)
{
    QString filePath=openningFileName.at(number);
   // m_channel->removeFile(filePath);
    m_workWidget = WorkFileWidget;
    m_jobType = JobUploadFile;
    channel_2->uploadFile(file,filePath, QSsh::SftpOverwriteExisting);
    handleRefreshClicked();

}

void SftpServer::timerEvent(QTimerEvent *event)
{
    //qDebug()<<"timer:"<<event->timerId();

    if(JobDownloadFile == m_jobType)
    {
        int iProgress = QFileInfo(m_currentLocalFilePath).size()*100/m_currentSize;
        if(iProgress > 99)
        {
            iProgress = 99;
        }
        m_progress->setValue(iProgress);
    }

    if(JobUploadFile == m_jobType)
    {
        m_jobUploadId = m_channel->statFile(m_currentRemoteFilePath);
    }
}
