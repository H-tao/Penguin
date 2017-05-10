/*调用流程：
 * 1.先通过setFileInfoList()传入文件列表
 * 2.再调用openFileSystem()
*/
#include "filesystem.h"
#include "ui_filesystem.h"

FileSystem::FileSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSystem)
{
    ui->setupUi(this);
    listWidget = new QListWidget(this);
//    qDebug() << listWidget->width();
    listWidget->resize(this->width(),this->height());
    Working();
}

FileSystem::FileSystem(QListWidget *fileWidget,int a)
{
    listWidget = fileWidget;
    Working();
}

void FileSystem::Working()
{
    QFileInfoList testFileInfoList;
    QFileInfo fileInfo(":/icons/bell.png");
    testFileInfoList.append(fileInfo);
    QFileInfo fileInfo2(":/icons/book.zip");
    testFileInfoList.append(fileInfo2);
    QFileInfo fileInfo3(":/icons/book.exe");
    testFileInfoList.append(fileInfo3);
    QFileInfo fileInfo4(":/icons/book.txt");
    testFileInfoList.append(fileInfo4);
    QFileInfo fileInfo5(":/icons/book.docx");
    testFileInfoList.append(fileInfo5);
    QFileInfo fileInfo6(":/icons/bell.cpp");
    testFileInfoList.append(fileInfo6);
    QFileInfo fileInfo7(":/icons/bell.html");
    testFileInfoList.append(fileInfo7);
    QFileInfo fileInfo8(":/icons/bell.h");
    testFileInfoList.append(fileInfo8);
    QFileInfo fileInfo9(":/icons/bell.gif");
    testFileInfoList.append(fileInfo9);
    setFileInfoList(testFileInfoList);
    openFileSystem();
}

void FileSystem::PrintLW()
{
    qDebug() << listWidget->width();
}

FileSystem::~FileSystem()
{
    delete ui;
}

QIcon FileSystem::getFileIcon(const QString &suffix) const
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
}

void FileSystem::openFileSystem()
{
     listWidget->setIconSize(QSize(48, 48));         // 设置单元项图片大小
     listWidget->setResizeMode(QListView::Adjust);   // 设置大小模式-可调节
     listWidget->setViewMode(QListView::IconMode);   // 设置显示模式
     listWidget->setMovement(QListView::Static);     // 设置单元项不可被拖动
     listWidget->setSpacing(10);                     // 设置单元项间距
    for(int i = 0; i < fileInfoList.count(); ++i)
    {
        QIcon icon = getFileIcon(fileInfoList.at(i).suffix());
        QString name = fileInfoList.at(i).fileName();

        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setIcon(icon);
        item->setText(name);
        listWidget->addItem(item);
    }
}

void FileSystem::setFileInfoList(QFileInfoList infoList)
{
    fileInfoList = infoList;
    qDebug() << fileInfoList.count();
}
