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
    initStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化
void MainWindow::initStyle()
{
    setUiDesignerStyle();   //设置UI界面风格
//    connectAction();    //连接所有Action信号槽
    ui->fileSystemWidget->setVisible(false);    //设置初始文件资源管理系统为不可见
    ShellTextEdit *textEdit = new ShellTextEdit(ui->tab_1);
    ui->firstTabLayout->addWidget(textEdit);
    //由于QTabWidget默认有两个Tab，所有需移除tabWidget的第二个Tab
    ui->tabWidget->removeTab(1);
}

//UI界面风格
void MainWindow::setUiDesignerStyle()
{
    setWindowTitle("小企鹅终端模拟软件");
    setWindowIcon(QIcon(QStringLiteral(":/images/penguin_32px.ico")));
}

//连接所有Action信号槽
void MainWindow::connectAction()
{

}


//打开文件管理系统Action
void MainWindow::on_openFileSystemAction_triggered()
{
    //如果文件管理系统已经打开
    if(isOpenFileSystem)
    {
        on_showFileSystemAction_triggered();
        return;
    }

    ui->filePathLineText->setText("d:\\");
    ui->filePathLineText->setAlignment(Qt::AlignLeft);
    ui->filePathLineText->setEnabled(false);
    ui->filePathLineText->setFocusPolicy(Qt::NoFocus);
    FileWidget *fileWidget = new FileWidget(ui->fileSystemWidget);
//    QGridLayout *Box = new QGridLayout;
//    Box->addWidget(fileWidget);
    ui->fileSystemWidgetLayout->addWidget(fileWidget);
    fileWidget->Working();
    ui->fileSystemWidget->setFrameShape(QFrame::NoFrame);
    on_showFileSystemAction_triggered();
    isOpenFileSystem = true;
}

//关闭文件管理系统Action
void MainWindow::on_closeFileSystemAction_triggered()
{
//    ui->gridLayout->removeWidget();
}

//显示文件管理系统Action
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
    }*/
}

//隐藏文件管理系统Action
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

//建立新连接Action
void MainWindow::on_newConnectionAction_triggered()
{

}

//增加新选项卡
void MainWindow::on_addTabAction_triggered()
{
    ShellTextEdit *textEdit = new ShellTextEdit(ui->tabWidget);
    ui->tabWidget->addTab(textEdit,"本地Shell");
    qDebug() << ui->tabWidget->count();
}

//关闭当前选项卡
void MainWindow::on_closeCurrentTabAction_triggered()
{
    if(ui->tabWidget->count() >= 1)
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    //选项卡全部关闭则关闭小企鹅
    if(ui->tabWidget->count() == 0)
        close();
}
