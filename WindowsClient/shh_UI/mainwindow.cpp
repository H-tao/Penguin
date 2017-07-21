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
    initWindowMenu();
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
    statusBar()->showMessage(tr("ready"));

    //由于QTabWidget默认有两个Tab，所以需移除tabWidget的第二个Tab
    ui->tabWidget->removeTab(1);
}

void MainWindow::initWindowMenu()
{
    pOpenSftpAct = new QAction(tr("Open Sftp Server"),this);
    ui->windowMenu->addAction(pOpenSftpAct);
    connect(pOpenSftpAct, SIGNAL(triggered(bool)), this, SLOT(openSftpServer()));
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
    if(newCon->exec()==QDialog::Accepted)
    {

        QSsh::SshConnectionParameters *sshPara;
        newCon->setText();
        sshPara=new QSsh::SshConnectionParameters();
        sshPara->host=newCon->getHost();
        sshPara->port=newCon->getPort();
        sshPara->userName=newCon->getuUserName();
        sshPara->password=newCon->getPassword();
        sshPara->authenticationType=QSsh::SshConnectionParameters::AuthenticationByPassword;
        sshPara->timeout=500;
        paraPool.append(sshPara);
        if(shellPool.size()<=ui->tabWidget->currentIndex())
        {
            //当前的页面无连接
            shellPool.append(new Shell(*sshPara,ui->tabWidget->currentIndex(),this));
            qDebug()<<"55566";

        }
        else
        {
            shellPool.replace(ui->tabWidget->currentIndex(),new Shell(*sshPara,ui->tabWidget->currentIndex(),this));
            qDebug()<<"6666";
        }
        //获得数据
        shellPool.at(ui->tabWidget->currentIndex())->run();
        //改变标签名
        ui->tabWidget->setTabText(shellPool.size()-1,newCon->getuUserName());

        //连接发送命令信号和接收
        connect(tabPagePool.at(shellPool.size()-1)->textEdit,SIGNAL(arguementDone(QString)),
                shellPool.at(shellPool.size()-1)->ptr,SLOT(showInfoFromRemote(QString)));

    }
//    QSsh::SshConnectionParameters *sshPara;
//    sshPara=new QSsh::SshConnectionParameters();
//    sshPara->host="39.108.78.252";
//    sshPara->port=22;
//    sshPara->userName="thishzw";
//    sshPara->password="Hzw5820212";
//    sshPara->authenticationType=QSsh::SshConnectionParameters::AuthenticationByPassword;
//    sshPara->timeout=500;
//    shellPool.append(new Shell(*sshPara,this));
//    shellPool.at(shellPool.size()-1)->run();
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
    qDebug() << "Info from remote:" << arguement;
    tabPagePool.at(winNo)->textEdit->append(arguement);
    /*
    TabPage *tabPage = reinterpret_cast<TabPage*>(ui->tabWidget->widget(winNo));   //TODO
    tabPage->textEdit->append(arguement);    //将远端命令输出显示
*/
}

void MainWindow::on_actionTest_triggered()
{
    QString i="help\n";
    shellPool.at(ui->tabWidget->currentIndex())->handleIn(i);
    //测试用函数
    QString l="cd test\n";
    shellPool.at(ui->tabWidget->currentIndex())->handleIn(l);
    QString l1="ls\n";
    shellPool.at(ui->tabWidget->currentIndex())->handleIn(l1);
}

void MainWindow::on_action_6_triggered()//断开连接
{
   int winNo=ui->tabWidget->currentIndex();
   shellPool.at(winNo)->disconnect();
   delete shellPool.at(winNo);
}

void MainWindow::on_action_4_triggered()
{
   int winNo=ui->tabWidget->currentIndex();
   if(winNo>shellPool.size()-1 && winNo>tabPagePool.size()-1)
       return ;
   shellPool.at(winNo)->reconnect();
}

void MainWindow::showInfoFromRemote(QString arguement)
{
    qDebug() << "testSlot arguement:" << arguement;
    arguement.remove('\r');
    arguement += "\n";
    shellPool.at(ui->tabWidget->currentIndex())->handleIn(arguement);
  //  tabPagePool.at(ui->tabWidget->currentIndex())->textEdit->append(arguement);
}

void MainWindow::openSftpServer()
{
    qDebug() << "openSftpServer clieked";
}
