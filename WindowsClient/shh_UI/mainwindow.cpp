#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiDesignerStyle();
    setTextEditSytel();

    //连接文件管理系统 打开、关闭、显示、隐藏 触发事件
    connect(ui->openFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_openFileSystemAction_triggered()));
    connect(ui->closeFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_closeFileSystemAction_triggered()));
    connect(ui->showFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_showFileSystemAction_triggered()));
    connect(ui->concealFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_concealFileSystemAction_triggered()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//UI界面风格
void MainWindow::uiDesignerStyle()
{
    setWindowTitle("小企鹅终端模拟软件");
    setWindowIcon(QIcon(QStringLiteral(":/images/penguin_32px.ico")));
}

void MainWindow::setTextEditSytel()
{
    //设置文本标签背景
    ui->textEdit->setStyleSheet("background-color:drak");
    //设置字体颜色
    QPalette pal = ui->textEdit->palette();
    //pal.setColor(QPalette::Background, QColor(0,0,0));
    pal.setColor(QPalette::Text, QColor(250,250,250));
    ui->textEdit->setPalette(pal);
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
//打开文件管理系统
void MainWindow::on_openFileSystemAction_triggered()
{
    //如果文件管理系统已经打开
    if(isOpenFileSystem)
    {
        on_showFileSystemAction_triggered();
        return;
    }

    //水平布局管理器（父布局管理器），垂直布局管理器（子布局管理器）
    //整个界面有水平和垂直两个布局管理，根布局为水平，子布局为垂直
    //将label部件加入垂直布局管理器中
    //再将一个或多个垂直布局管理器加入水平布局管理器
    QWidget *containWidget = new QWidget;
    QVBoxLayout *VBox = new QVBoxLayout(containWidget);
    QHBoxLayout *HBox = new QHBoxLayout;
    QHBoxLayout *HBox_1 = new QHBoxLayout;
    for(int i = 1; i <= 15; ++i)
    {
        QLabel *label = new QLabel;
//        //label的形式
//        label->setText(tr("文件%1").arg(i+1));
//        label->setStyleSheet("background-color: rgb(255, 170, 0);");
//        //border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0); StyleSheet参数,边框宽、类型、颜色
        QImage *image = new QImage();
        image->load(":/images/penguin_32px.png");
        label->setPixmap(QPixmap::fromImage(*image));
        label->resize(image->width(),image->height());
        qDebug() << image->width() << " " << image->height();
        if(i<11)      //当label数量大于10时，另外增加一个水平布局管理器
        {
            HBox->addWidget(label);
        }
        else
            HBox_1->addWidget(label);
    }
    VBox->addLayout(HBox);
    VBox->addLayout(HBox_1);
    VBox->setSpacing(10);
//    containWidget->setLayout(VBox);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(containWidget);

    isOpenFileSystem = true;
}

//关闭文件管理系统
void MainWindow::on_closeFileSystemAction_triggered()
{

}

//显示文件管理系统
void MainWindow::on_showFileSystemAction_triggered()
{
    if(isShowFileSystem == false)
    {
       ui->scrollArea->setVisible(true);
       //设置显示文件管理系统的值为真
       isShowFileSystem = true;
       return;
    }
    //如果已经文件管理系统打开
/*
    if(isShowFileSystem == true)
    {
        //提示对话框
        QMessageBox::information(this, tr("提示"), tr("文件管理系统已经显示！"), QMessageBox::Ok);
        qDebug() << "``````";
        return;
    }*/
}

//隐藏文件管理系统
void MainWindow::on_concealFileSystemAction_triggered()
{
    if(isShowFileSystem == false)
        return;

    if(isShowFileSystem == true)
    {
        ui->scrollArea->setVisible(false);
        isShowFileSystem = false;
        return;
    }
}
