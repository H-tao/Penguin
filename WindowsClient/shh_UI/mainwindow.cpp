#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include<QDebug>
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
    /*
    textEdit = new ShellTextEdit(ui->tab_1);
    ui->firstTabLayout->addWidget(textEdit);
    */
    TabPage *tabPage = new TabPage;
    tabPagePool.append(tabPage);
    ui->firstTabLayout->addWidget(tabPage);

    //由于QTabWidget默认有两个Tab，所以需移除tabWidget的第二个Tab
    ui->tabWidget->removeTab(1);
}

//UI界面风格
void MainWindow::setUiDesignerStyle()
{
    setWindowTitle(tr("小企鹅终端模拟软件"));
    setWindowIcon(QIcon(QStringLiteral(":/images/penguin_32px.ico")));
}

//连接所有Action信号槽
void MainWindow::connectAction()
{

}


//打开文件管理系统Action
void MainWindow::on_openFileSystemAction_triggered()
{
    if(tabPagePool.at(ui->tabWidget->currentIndex()) == NULL)
    {
        //提示对话框
        QMessageBox::information(this, tr("提示"), tr("指针为空！"), QMessageBox::Ok);
    }

    tabPagePool.at(ui->tabWidget->currentIndex())->openFileSystem();
}

//关闭文件管理系统Action
void MainWindow::on_closeFileSystemAction_triggered()
{
    tabPagePool.at(ui->tabWidget->currentIndex())->closeFileSystem();
}

//显示文件管理系统Action
void MainWindow::on_showFileSystemAction_triggered()
{
    tabPagePool.at(ui->tabWidget->currentIndex())->showFileSystem();
}

//隐藏文件管理系统Action
void MainWindow::on_concealFileSystemAction_triggered()
{
    tabPagePool.at(ui->tabWidget->currentIndex())->concealFileSystem();
}

//建立新连接Action
void MainWindow::on_newConnectionAction_triggered()
{
    NewConnection *newCon;
    newCon=new NewConnection();
    newCon->exec();
    newCon->show();
    QSsh::SshConnectionParameters *sshPara;
    sshPara=new QSsh::SshConnectionParameters();
    sshPara->host="39.108.78.252";
    sshPara->port=22;
    sshPara->userName="thishzw";
    sshPara->password="Hzw5820212";
    sshPara->authenticationType=QSsh::SshConnectionParameters::AuthenticationByPassword;
    sshPara->timeout=500;
    shell=new Shell(*sshPara,this);
    shell->run();
    qDebug()<<sshPara->host;
    //获得数据
    delete newCon;
}

//增加新选项卡Action
void MainWindow::on_addTabAction_triggered()
{
    /* old
    ShellTextEdit *textEdit = new ShellTextEdit(ui->tabWidget);
    ui->tabWidget->addTab(textEdit,"本地Shell");
    */

    TabPage *tabPage = new TabPage(ui->tabWidget);
    ui->tabWidget->addTab(tabPage,"本地Shell");
    tabPagePool.append(tabPage);                        //加入TabPage池
    qDebug() << ui->tabWidget->count();
}

//关闭当前选项卡Action
void MainWindow::on_closeCurrentTabAction_triggered()
{
    if(ui->tabWidget->count() >= 1)
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    //选项卡全部关闭则关闭小企鹅
    if(ui->tabWidget->count() == 0)
        close();
}

void MainWindow::outToShell(int winNo, QString arguement)
{
    tabPagePool.at(winNo)->textEdit->append(arguement);
    /*
    TabPage *tabPage = reinterpret_cast<TabPage*>(ui->tabWidget->widget(winNo));   //TODO
    tabPage->textEdit->append(arguement);    //将远端命令输出显示
*/
}
