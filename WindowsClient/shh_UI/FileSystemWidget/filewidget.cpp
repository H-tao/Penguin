#include "filewidget.h"

FileWidget::FileWidget(QWidget *parent) :
    QListWidget(parent)
{
    setIconSize(QSize(48, 48));         // 设置单元项图片大小
    setResizeMode(QListView::Adjust);   // 设置大小模式-可调节
    setViewMode(QListView::IconMode);   // 设置显示模式
    setMovement(QListView::Static);     // 设置单元项不可被拖动
    setSpacing(10);                     // 设置单元项间距
}

FileWidget::~FileWidget()
{
//    delete this;
//    this = nullptr;
}

//打开文件系统
void FileWidget::openFileSystem()
{
    emit openClicked();
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
        if(fi.type == QSsh::FileTypeRegular)
            icon = getFileIcon(fi.name);
        if(fi.type == QSsh::FileTypeDirectory)
            icon = getFolderIcon();

        QListWidgetItem *item = new QListWidgetItem(this);
        item->setIcon(icon);
        item->setText(fi.name);
        this->addItem(item);
    }
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
