#include "filesystemwidget.h"
#include "ui_filesystemwidget.h"
#include<QDir>
#include<QUrl>

FileSystemWidget::FileSystemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSystemWidget)
{
    ui->setupUi(this);

    findIconPath("a");
//    ui->scrollArea->setWidgetResizable(false);
    QFileInfo fileInfo(":/icons/bell.png");
    testFileInfoList.append(fileInfo);
    QFileInfo fileInfo2(":/icons/book.book");
    testFileInfoList.append(fileInfo2);
    QFileInfo fileInfo3(":/icons/book.png");
    testFileInfoList.append(fileInfo3);
    QFileInfo fileInfo4(":/icons/book.png");
    testFileInfoList.append(fileInfo4);
    QFileInfo fileInfo5(":/icons/book.png");
    testFileInfoList.append(fileInfo5);
    QFileInfo fileInfo6(":/icons/bell.book");
    testFileInfoList.append(fileInfo6);
    openFileSystem(testFileInfoList);
}

FileSystemWidget::~FileSystemWidget()
{
    delete ui;
}

/*文件管理系统显示流程
1、通过shell获取当前路径下的file文件列表，fileInfoList
2、获取本地icon图标文件列表，iconInfoList
3、扫描整个文件，当文件后缀和图标前缀匹配时，将图标路径返回，否则返回空路径
if(fileInfoList.at(i).suffix() == iconInfoList.at(j).baseName())
    return iconInfoList.at(j).filePath()
4、判断图标路径是否为空，不为空就取icon图标文件并显示在文件管理系统，
否则查看普通文件图标是否存在，存在则显示，不存在显示黄色背景并给予用户提示
缺点：如果目录中文件数量过多或者图标数量过多，会拖慢系统进程
*/
void FileSystemWidget::openFileSystem(QFileInfoList fileInfoList)
{
    QString iconPath;
    for(int i = 0; i < fileInfoList.length(); ++i)
    {
        //通过后缀获取图标路径以加载至文件管理系统
        iconPath = findIconPath(fileInfoList.at(i).suffix());
        EnhancedLabel *fileLabel = new EnhancedLabel(this);
        fileLabel->set(iconPath, fileInfoList.at(i).fileName());
        fileLabels.append(fileLabel);
    }
}

/*图片显示随Widgets大小改变而改变*/
void FileSystemWidget::resizeEvent(QResizeEvent *)
{
    setGeometryOfImageLabel();
}

//设置EnhancedLabel的位置
void FileSystemWidget::setGeometryOfImageLabel()
{
    for(EnhancedLabel *l : fileLabels)
    {
        int i = fileLabels.indexOf(l);  //EnhancedLabel的第i个

        //行最大数 = widget.width / img.width
        int rowMax = 0;
        rowMax = this->width() / l->width();
        rowMax = (rowMax > fileLabels.size())? fileLabels.size() : rowMax;

        int x = i % rowMax;     //第i个label所在行
        int y = i / rowMax;        //第i个label所在列
        //图标间隔

        int spacing = (this->width() - rowMax * l->width()) / rowMax;
//        qDebug() << "ui->scrollAreaWidgetContents->width(): " << ui->scrollAreaWidgetContents->width();
//        qDebug() << "Spacing: " << spacing;

        //设置图标位置
        l->setGeometry((l->width()+spacing)*x, l->height()*y, l->width(), l->height());
    }
}

//获取图标路径，图标为*.png类型
QString FileSystemWidget::findIconPath(QString FileSuffix)
{
    //icons的文件列表
    QDir dir(":/icons");

    //主要过滤器的参数，不排序
    QStringList filter;
    filter << "*.png";  //多个后缀名过滤filter << "*.png" << "*.dat";
    dir.setNameFilters(filter);
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList iconInfoList = dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name);

    //在icons文件夹中找到图标baseName前缀与文件suffix后缀名相同的图标路径
    //用List的indexOf()找
    QString indexPath = iconInfoList.at(0).path() + "/" + FileSuffix +".png";
    QFileInfo indexInfo(indexPath);
    int i = iconInfoList.indexOf(indexInfo);    //索引
    if(i >= 0)  //大于等于0，成功获取路径,并返回
        return iconInfoList.at(i).filePath();
    //小于0，获取路径失败
    return ":/icons/folder.png";
}
