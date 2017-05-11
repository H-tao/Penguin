#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiDesignerStyle();
    setTextEditSytel();
    ui->fileSystemWidget->setVisible(false);

    //连接文件管理系统 打开、关闭、显示、隐藏 触发事件
    connect(ui->openFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_openFileSystemAction_triggered()));
    connect(ui->closeFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_closeFileSystemAction_triggered()));
    connect(ui->showFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_showFileSystemAction_triggered()));
    connect(ui->concealFileSystemAction, SIGNAL(triggered(bool)), this, SLOT(on_concealFileSystemAction_triggered()));
    connect(ui->newConnectionAction,SIGNAL(triggered(bool)),this,SLOT(on_newConnectionAction_triggered()));
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

//文本框风格
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

//打开文件管理系统
void MainWindow::on_openFileSystemAction_triggered()
{
    //如果文件管理系统已经打开
    if(isOpenFileSystem)
    {
        on_showFileSystemAction_triggered();
        return;
    }


    ui->filePathLineText->setText("D:\\QtDecements\\build-shh_UI-Desktop_Qt_5_7_0_MinGW_32bit-Release\\release");
    ui->filePathLineText->setAlignment(Qt::AlignLeft);
    ui->filePathLineText->setEnabled(false);
    ui->filePathLineText->setFocusPolicy(Qt::NoFocus);
    FileWidget *fileWidget = new FileWidget(ui->fileSystemWidget);
//    QGridLayout *Box = new QGridLayout;
//    Box->addWidget(fileWidget);
    ui->gridLayout->addWidget(fileWidget);
    fileWidget->Working();
//    qDebug() << fileWidget->width();
//    qDebug() << ui->fileSystemWidget->width();
    isOpenFileSystem = true;
}

//关闭文件管理系统
void MainWindow::on_closeFileSystemAction_triggered()
{
//    ui->gridLayout->removeWidget();
}

//显示文件管理系统
void MainWindow::on_showFileSystemAction_triggered()
{
    if(isShowFileSystem == false)
    {
       ui->fileSystemWidget->setVisible(true);
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
        ui->fileSystemWidget->setVisible(false);
        isShowFileSystem = false;
        return;
    }
}

void MainWindow::on_newConnectionAction_triggered()
{
//    FileSystem *fileSystem = new FileSystem(this);
//    fileSystem->show();
}
