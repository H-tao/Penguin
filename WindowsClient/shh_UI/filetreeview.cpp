#include "filetreeview.h"

FileTreeView::FileTreeView(QWidget *parent) :
    QTreeView(parent)
{
    this->initialize();
    this->initOperaMenu();
}

FileTreeView::~FileTreeView()
{

}

void FileTreeView::initialize()
{
    this->setSortingEnabled(true);
    this->setAlternatingRowColors(true);

    m_model = new QStandardItemModel(0, 4, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Name"), Qt::DisplayRole);
    m_model->setHeaderData(1, Qt::Horizontal, tr("Permission"), Qt::DisplayRole);
    m_model->setHeaderData(2, Qt::Horizontal, tr("Size"), Qt::DisplayRole);
    m_model->setHeaderData(3, Qt::Horizontal, tr("Type"), Qt::DisplayRole);

    this->setModel(m_model);
    this->setColumnWidth(0, 150);
    this->setColumnWidth(2, 150);
    this->setColumnWidth(3, 150);

    this->header()->setSortIndicator(0, Qt::AscendingOrder);
    connect(this->header(), &QHeaderView::sectionClicked,
                [this](int column)
    {
        this->sortByColumn(column, this->header()->sortIndicatorOrder());
    });
}

void FileTreeView::initOperaMenu()
{
    m_pOperaMenu = new QMenu(this);
    m_pNewMenu = new QMenu(tr("new"),this);

    m_pActOpen = new QAction(tr("open"),this);
    m_pActUpload = new QAction(tr("upload"),this);
    m_pActDownload = new QAction(tr("download"),this);
    m_pActHome = new QAction(tr("home"),this);
    m_pActNewFolder = new QAction(tr("new folder"),this);
    m_pActNewFile = new QAction(tr("new file"),this);
    m_pActUp = new QAction(tr("up"),this);
    m_pActRename = new QAction(tr("rename"),this);
    m_pActRefresh = new QAction(tr("refresh"),this);
    m_pActDelete = new QAction(tr("delete"),this);

    m_pNewMenu->addAction(m_pActNewFolder);
    m_pNewMenu->addAction(m_pActNewFile);

    m_pOperaMenu->addAction(m_pActOpen);
    m_pOperaMenu->addAction(m_pActUp);
    m_pOperaMenu->addAction(m_pActHome);
    m_pOperaMenu->addAction(m_pActRefresh);
    m_pOperaMenu->addSeparator();
    m_pOperaMenu->addMenu(m_pNewMenu);
    m_pOperaMenu->addAction(m_pActDelete);
    m_pOperaMenu->addAction(m_pActRename);
    m_pOperaMenu->addSeparator();
    m_pOperaMenu->addAction(m_pActUpload);
    m_pOperaMenu->addAction(m_pActDownload);

    m_pMarginMenu = new QMenu(this);
    m_pMarginMenu->addAction(m_pActUp);
    m_pMarginMenu->addAction(m_pActHome);
    m_pMarginMenu->addAction(m_pActRefresh);
    m_pMarginMenu->addMenu(m_pNewMenu);
    m_pMarginMenu->addAction(m_pActUpload);
//    m_pMarginMenu->addAction(m_pActDownload);

    connect(m_pActOpen, SIGNAL(triggered()), this, SLOT(clickedOpen()));
    connect(m_pActUpload, SIGNAL(triggered()), this, SLOT(clickedUpload()));
    connect(m_pActDownload, SIGNAL(triggered()), this, SLOT(clickedDownload()));
    connect(m_pActUp, SIGNAL(triggered()), this, SLOT(clickedUp()));
    connect(m_pActHome, SIGNAL(triggered()), this, SLOT(clickedHome()));
    connect(m_pActRefresh, SIGNAL(triggered()), this, SLOT(clickedRefresh()));
    connect(m_pActNewFolder, SIGNAL(triggered()), this, SLOT(clickedNewFolder()));
    connect(m_pActNewFile, SIGNAL(triggered()), this, SLOT(clickedNewFile()));
    connect(m_pActDelete, SIGNAL(triggered()), this, SLOT(clickedDelete()));
    connect(m_pActRename, SIGNAL(triggered()), this, SLOT(clickedRename()));
//    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuView(QPoint)));
    connect(this,&FileTreeView::customContextMenuRequested, this, &FileTreeView::customMenuView);
}

void FileTreeView::openFileSystem()
{
    emit openFileSystemClicked();
}

void FileTreeView::refreshDirectory(const QList<QSsh::SftpFileInfo> &fiList)
{
    m_model->removeRows(0, m_model->rowCount());

    foreach(const QSsh::SftpFileInfo &fi, fiList)
    {
//        qDebug() << fi.name << " " << fi.permissions << " " << fi.type << " " << fi.size;
//        qDebug() << "Permission : "  << " " << fi.permissionsValid;

        if(fi.name == ".." || fi.name == ".")
        {
            continue;
        }

        int rows = m_model->rowCount();
        m_model->insertRow(rows);
        m_model->setData(m_model->index(rows, 0, QModelIndex()), fi.name, Qt::DisplayRole);
        m_model->setData(m_model->index(rows, 0, QModelIndex()),
                         (fi.permissionsValid == true ? "can access" : "cann't access"), Qt::StatusTipRole);
        m_model->setData(m_model->index(rows, 0, QModelIndex()), fi.permissionsValid, Qt::ToolTipRole);

        if(fi.type == QSsh::FileTypeRegular)
        {
            m_model->setData(m_model->index(rows, 0, QModelIndex()), getFileIcon(fi.name), Qt::DecorationRole);
            m_model->setData(m_model->index(rows, 1, QModelIndex()), /*"-" + */getPermissions(fi.permissions), Qt::DisplayRole);
            m_model->setData(m_model->index(rows, 2, QModelIndex()), getSizeFromByte(fi.size), Qt::DisplayRole);
            m_model->setData(m_model->index(rows, 3, QModelIndex()), getFileType(fi.name), Qt::DisplayRole);
        }
        if(fi.type == QSsh::FileTypeDirectory)
        {
            m_model->setData(m_model->index(rows, 0, QModelIndex()), getFolderIcon(), Qt::DecorationRole);
            m_model->setData(m_model->index(rows, 1, QModelIndex()), /*"d" + */getPermissions(fi.permissions), Qt::DisplayRole);
            m_model->setData(m_model->index(rows, 2, QModelIndex()), ""/*getSizeFromByte(fi.size)*/, Qt::DisplayRole);
            m_model->setData(m_model->index(rows, 3, QModelIndex()), getFolderType(), Qt::DisplayRole);
        }
    }

    this->sortByColumn(this->header()->sortIndicatorSection(), this->header()->sortIndicatorOrder());
}

void FileTreeView::customMenuView(QPoint pt)
{
    qDebug() << "customMenuView";
    QModelIndex index = this->indexAt(pt);
    if(index.isValid())
        m_pOperaMenu->exec(QCursor::pos());
    else
        m_pMarginMenu->exec(QCursor::pos());

}

//重新设置当前行
void FileTreeView::setThisFileInfo()
{
    int row = this->currentIndex().row();
    thisFileName = m_model->data(m_model->index(row, 0, QModelIndex()), Qt::DisplayRole).toString();
    thisFilePermission = m_model->data(m_model->index(row, 1, QModelIndex()), Qt::DisplayRole).toString();
    thisFileSize = m_model->data(m_model->index(row, 2, QModelIndex()), Qt::DisplayRole).toString();
    thisFileType = m_model->data(m_model->index(row, 3, QModelIndex()), Qt::DisplayRole).toString();
    thisFilePermissionValid = m_model->data(m_model->index(row, 0, QModelIndex()), Qt::ToolTipRole).toBool();
}

void FileTreeView::clickedOpen()
{
//    this->setThisFileInfo();
    emit openClicked(thisFileName, thisFileType, thisFileSize);
}

void FileTreeView::clickedUpload()
{
    emit uploadClicked();
}

void FileTreeView::clickedDownload()
{
//    this->setThisFileInfo();
    emit downloadClicked(thisFileName, thisFileType, thisFileSize);
}

void FileTreeView::clickedUp()
{
    emit upClicked();
}

void FileTreeView::clickedHome()
{
    emit homeClicked();
}

void FileTreeView::clickedNewFolder()
{
    emit newFolderClicked();
}

void FileTreeView::clickedNewFile()
{
    emit newFileClicked();
}

void FileTreeView::clickedDelete()
{
//    this->setThisFileInfo();
    emit deleteClicked(thisFileName, thisFileType);
}

void FileTreeView::clickedRename()
{
//    this->setThisFileInfo();
    emit renameClicked(thisFileName);
}

void FileTreeView::clickedRefresh()
{
    emit refreshClicked();
}

bool FileTreeView::isFileExisted(QString fileName)
{
    QList<QStandardItem *> list = m_model->findItems(fileName, Qt::MatchExactly);
    if(list.isEmpty())
        return false;
    else
        return true;
}

void FileTreeView::mousePressEvent(QMouseEvent *event)
{
    this->setThisFileInfo();
    if(event->button() == Qt::RightButton)
        emit this->customContextMenuRequested(event->pos());

    QTreeView::mousePressEvent(event);
}

void FileTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->setThisFileInfo();
    if(event->button() == Qt::LeftButton)
        emit this->openClicked(thisFileName, thisFileType, thisFileSize);
    if(event->button() == Qt::RightButton)
        event->ignore();
}

void FileTreeView::keyPressEvent(QKeyEvent *event)
{
    this->setThisFileInfo();
    if(event->key() == Qt::Key_Backspace)
        emit this->upClicked();
    if(event->key() == Qt::Key_F5)
        emit this->refreshClicked();
    if(event->key() == Qt::EnterKeyDone)
        emit this->openClicked(thisFileName, thisFileType, thisFileSize);

    QTreeView::keyPressEvent(event);
}

void FileTreeView::sortByColumn(int column, Qt::SortOrder order)
{
    QTreeView::sortByColumn(column, order);

    int rows = m_model->rowCount(QModelIndex());
    int folderCount = 0;

    QString selectedName = m_model->data(m_model->index(this->currentIndex().row(), 0), Qt::DisplayRole).toString();

    // sort
    for(int i = 0; i < rows; ++i)
    {
        QString fileType = m_model->data(m_model->index(i, 3), Qt::DisplayRole).toString();
        if(fileType == getFolderType())
        {
            m_model->insertRow(folderCount, m_model->takeRow(i));
            ++folderCount;
        }
    }

    // reset the seleced row
    for(int i = 0; i < rows; ++i)
    {
        QString fileName = m_model->data(m_model->index(i, 0), Qt::DisplayRole).toString();
        if(fileName == selectedName)
        {
            this->setCurrentIndex(m_model->index(i, 0));
            break;
        }
    }
}
