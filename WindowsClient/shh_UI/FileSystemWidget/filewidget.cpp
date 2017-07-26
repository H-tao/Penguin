#include "filewidget.h"
QSize itemSize(50,60);

FileWidget::FileWidget(QWidget *parent) :
    QListWidget(parent)
{
    setIconSize(QSize(48, 48));         // 设置单元项图片大小
    setResizeMode(QListView::Adjust);   // 设置大小模式-可调节
    setViewMode(QListView::IconMode);   // 设置显示模式
    setMovement(QListView::Static);     // 设置单元项不可被拖动
    setSpacing(2);                     // 设置单元项间距
//    setTextElideMode(Qt::ElideRight);   // 设置文本
    setFlow(QListView::LeftToRight);
    this->setSortingEnabled(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    initOperaMenu();

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleItemDoubleClicked(QListWidgetItem*)));
}

FileWidget::~FileWidget()
{
//    delete this;
//    this = nullptr;
}

void FileWidget::initOperaMenu()//右键菜单设置
{
    m_pOperaMenu = new QMenu();

    m_pActOpen = new QAction(tr("open"),this);
    m_pActUpload = new QAction(tr("upload"),this);
    m_pActDownload = new QAction(tr("download"),this);
    m_pActHome = new QAction(tr("home"),this);
    m_pActNew = new QAction(tr("new"),this);
    m_pActUp = new QAction(tr("up"),this);
    m_pActRename = new QAction(tr("rename"),this);
    m_pActRefresh = new QAction(tr("refresh"),this);
    m_pActDelete = new QAction(tr("delete"),this);

    m_pOperaMenu->addAction(m_pActOpen);
    m_pOperaMenu->addAction(m_pActUp);
    m_pOperaMenu->addAction(m_pActHome);
    m_pOperaMenu->addAction(m_pActRefresh);
    m_pOperaMenu->addSeparator();
    m_pOperaMenu->addAction(m_pActNew);
    m_pOperaMenu->addAction(m_pActDelete);
    m_pOperaMenu->addAction(m_pActRename);
    m_pOperaMenu->addSeparator();
    m_pOperaMenu->addAction(m_pActUpload);
    m_pOperaMenu->addAction(m_pActDownload);

    connect(m_pActOpen, SIGNAL(triggered()), this, SLOT(clickedOpen()));
    connect(m_pActUpload, SIGNAL(triggered()), this, SLOT(clickedUpload()));
    connect(m_pActDownload, SIGNAL(triggered()), this, SLOT(clickedDownload()));
    connect(m_pActUp, SIGNAL(triggered()), this, SLOT(clickedUp()));
    connect(m_pActHome, SIGNAL(triggered()), this, SLOT(clickedHome()));
    connect(m_pActRefresh, SIGNAL(triggered()), this, SLOT(clickedRefresh()));
    connect(m_pActNew, SIGNAL(triggered()), this, SLOT(clickedNewFolder()));
    connect(m_pActDelete, SIGNAL(triggered()), this, SLOT(clickedDelete()));
    connect(m_pActRename, SIGNAL(triggered()), this, SLOT(clickedRename()));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuView(QPoint)));

}

//打开文件系统
void FileWidget::openFileSystem()
{
    emit openFileSystemClicked();
}

//传入文件信息列表
void FileWidget::setFileInfoList(const QList<QSsh::SftpFileInfo> &fiList)
{
    fileInfoList = fiList;
}

void FileWidget::refreshDirectory(const QList<QSsh::SftpFileInfo> &fiList)
{
    // clear all the item
    this->clear();

    QIcon icon(":/icons/folder.png");
    foreach (const QSsh::SftpFileInfo &fi, fiList)
    {

        qDebug()<<fi.name<<" "<<fi.size<<" "<<fi.type<<" "<<fi.permissions;

        if(fi.name == ".." || fi.name == ".")
        {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(this);
        item->setData(Qt::DisplayRole, fi.name);

        if(fi.type == QSsh::FileTypeRegular)
        {
            item->setData(Qt::DecorationRole, getFileIcon(fi.name));
            item->setData(Qt::ToolTipRole, getSizeFromByte(fi.size));
            item->setData(Qt::WhatsThisRole, getFileType(fi.name));
            item->setData(Qt::StatusTipRole, getPermissions(fi.permissions));
        }
        if(fi.type == QSsh::FileTypeDirectory)
        {
            item->setData(Qt::DecorationRole, getFolderIcon());
            item->setData(Qt::ToolTipRole, "Unknown");
            item->setData(Qt::WhatsThisRole, getFolderType());
            item->setData(Qt::StatusTipRole, getPermissions(fi.permissions));
        }

        item->setSizeHint(itemSize);

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

        this->addItem(item);
    }
}


void FileWidget::customMenuView(QPoint pt)
{
    qDebug() << "customMenuView";
    QListWidgetItem* cur=this->itemAt(pt);
    if(cur==NULL) return;

    m_pOperaMenu->exec(QCursor::pos());
    qDebug() << "exec";
}

void FileWidget::clickedOpen()
{
    qDebug()<<"open";
    if(this->currentItem()->text() == "")
        return;
    // text() == fileName
    emit openClicked(this->currentItem()->text(), this->currentItem()->whatsThis(), this->currentItem()->toolTip());
}

void FileWidget::clickedUp()
{
    qDebug()<<"break";
    emit upClicked();
}

void FileWidget::clickedHome()
{
    qDebug()<<"break home";
    emit homeClicked();
}

void FileWidget::clickedRefresh()
{
    qDebug()<<"refresh";
    emit refreshClicked();
}

void FileWidget::clickedNewFolder()
{
    qDebug()<<"new";
}

void FileWidget::clickedDelete()
{
    qDebug()<<"delete";
    emit deleteClicked(this->currentItem()->text(), this->currentItem()->whatsThis());
}

void FileWidget::clickedRename()
{
    qDebug()<<"rename";
    this->currentItem()->setSelected(true);
}

void FileWidget::clickedDownload()
{
    qDebug()<<"download";
    emit downloadClicked(this->currentItem()->text(), this->currentItem()->whatsThis(), this->currentItem()->toolTip());
}

void FileWidget::clickedUpload()
{
    qDebug()<<"upload";
}

//获取文件图标
/*
QIcon FileWidget::getFileIcon(const QString &suffix) const
{
    QFileIconProvider provider;
    QIcon icon;
    QString strTemlateName = QDir::tempPath() + QDir::separator() +
            QCoreApplication::applicationName() + "_XXXXXX." + suffix;
    QTemporaryFile tmpFile(strTemlateName);     //建立临时文件
    tmpFile.setAutoRemove(false);       //设置临时文件为不自动删除

    if(tmpFile.open())
    {
        tmpFile.close();
        icon = provider.icon(QFileInfo(tmpFile.fileName()));
    }
    else
    {
        qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
    }

    return icon;
}*/

void FileWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
        emit upClicked();
    QListWidget::keyPressEvent(event);
}

//void FileWidget::mousePressEvent(QMouseEvent *event)
//{
//    QListWidget::mousePressEvent(event);
//}

void FileWidget::mouseMoveEvent(QMouseEvent *event)
{
    QListWidget::mouseMoveEvent(event);
}

void FileWidget::handleItemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "handleItemDoubleClicked";
    emit openClicked(item->text(), item->whatsThis(), item->toolTip());
}
