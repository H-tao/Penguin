#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include<QDebug>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initStyle();
    initWindowMenu();
    this->setAcceptDrops(true);
    LinuxColor.insert(30,"black");
    LinuxColor.insert(31,"red");
    LinuxColor.insert(32,"green");
    LinuxColor.insert(33,"yellow");
    LinuxColor.insert(34,"blue");
    LinuxColor.insert(35,"purple");
    LinuxColor.insert(36,"skyblue");
    LinuxColor.insert(37,"white");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化
void MainWindow::initStyle()
{
    setUiDesignerStyle();   //设置UI界面风格
    TabPage *tabPage = new TabPage;
    tabPagePool.append(tabPage);
    ui->firstTabLayout->addWidget(tabPage);
    statusBar()->showMessage(tr("ready"));

    //由于QTabWidget默认有两个Tab，所以需移除tabWidget的第二个Tab
    ui->tabWidget->removeTab(1);
    ui->tabWidget->setTabsClosable(true);
    qApp->setPalette(QPalette(QColor("#444444")));
    openStyleSheet();

    connect(ui->tabWidget->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MainWindow::initWindowMenu()
{
    pOpenSftpAct = new QAction(tr("Open Sftp Server"),this);
    pCloseSftpAct = new QAction(tr("close Sftp Server"),this);
    pOpenTreeView = new QAction(tr("Open TreeView"),this);

    ui->windowMenu->addAction(pOpenSftpAct);
    ui->windowMenu->addAction(pCloseSftpAct);
    ui->windowMenu->addAction(pOpenTreeView);

    connect(pOpenSftpAct, SIGNAL(triggered(bool)), this, SLOT(openSftpServer()));
    connect(pCloseSftpAct, SIGNAL(triggered(bool)), this, SLOT(closeSftpServer()));
    connect(pOpenTreeView, SIGNAL(triggered(bool)), this, SLOT(openTreeView()));
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
    if(sftpPool.size() < (getCurrentIndex() + 1))
        newSftpServer();

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
    if(sftpPool.size() < getCurrentIndex())
        newSftpServer();

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
        sshPara=new QSsh::SshConnectionParameters();
        if(newCon->isPasswordConnect())
        {
            sshPara->host=newCon->getHost();
            sshPara->port=newCon->getPort();
            sshPara->userName=newCon->getuUserName();
            sshPara->password=newCon->getPassword();
            sshPara->authenticationType=QSsh::SshConnectionParameters::AuthenticationByPassword;
        }
        else
        {
            sshPara->host = newCon->getHost();
            sshPara->port = newCon->getPort();
            sshPara->userName = newCon->getuUserName();
            sshPara->privateKeyFile = newCon->getPrivateKey();
            sshPara->authenticationType = QSsh::SshConnectionParameters::AuthenticationByKey;
        }
        sshPara->timeout=500;
        if(shellPool.size()<=ui->tabWidget->currentIndex())
        {
            //当前的页面无连接
            shellPool.append(new Shell(*sshPara,ui->tabWidget->currentIndex(),this));
            //qDebug()<<"55566";
            paraPool.append(sshPara);

            //获得数据
            shellPool.at(ui->tabWidget->currentIndex())->run();
            //改变标签名
            ui->tabWidget->setTabText(getCurrentIndex(),newCon->getuUserName());
            //连接发送命令信号和接收
            connect(tabPagePool.at(getCurrentIndex())->textEdit,SIGNAL(arguementDone(QString)),
                    shellPool.at(getCurrentIndex())->ptr,SLOT(showInfoFromRemote(QString)));
            connect(tabPagePool.at(getCurrentIndex())->textEdit,SIGNAL(arguementDone(QByteArray)),
                    shellPool.at(getCurrentIndex())->ptr,SLOT(showInfoFromRemote(QByteArray)));
        }
        else
        {
            on_addTabAction_triggered();/*
            shellPool.replace(ui->tabWidget->currentIndex(),new Shell(*sshPara,ui->tabWidget->currentIndex(),this));
            paraPool.replace(getCurrentIndex(),sshPara);
            */
            shellPool.append(new Shell(*sshPara,shellPool.size(),this));
            paraPool.append(sshPara);
            //qDebug()<<"6666";

            //获得数据
            shellPool.at(shellPool.size()-1)->run();
            //改变标签名
            ui->tabWidget->setTabText(shellPool.size()-1,newCon->getuUserName());
            //连接发送命令信号和接收
            connect(tabPagePool.at(shellPool.size()-1)->textEdit,SIGNAL(arguementDone(QString)),
                    shellPool.at(shellPool.size()-1)->ptr,SLOT(showInfoFromRemote(QString)));
        }
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
    //qDebug() << ui->tabWidget->count();
}

//关闭当前选项卡Action
void MainWindow::on_closeCurrentTabAction_triggered()
{
    closeTab(getCurrentIndex());
}

void MainWindow::outToShell(int winNo, QString arguement)
{
    if(arguement.isEmpty())
        return;
    qDebug() << "Info from remote:" << arguement;
    colorDeal(arguement);
    tabPagePool.at(winNo)->textEdit->append(arguement);

}

void MainWindow::on_actionTest_triggered()
{
//    QString i="help\n";
//    shellPool.at(getCurrentIndex())->handleIn(i);
//    //测试用函数
//    QString l="cd test\n";
//    shellPool.at(getCurrentIndex())->handleIn(l);
//    QString l1="ls\n";
//    shellPool.at(getCurrentIndex())->handleIn(l1);
    this->tabPagePool.at(0)->textEdit->clearTheKeyin();
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
    //qDebug() << "testSlot arguement:" << arguement;
    shellPool.at(getCurrentIndex())->handleIn(arguement);
  //  tabPagePool.at(ui->tabWidget->currentIndex())->textEdit->append(arguement);
}
void MainWindow::showInfoFromRemote(QByteArray arguement)
{
    shellPool.at(getCurrentIndex())->handleIn(arguement);

}
void MainWindow::openSftpServer()
{
    //qDebug() << "openSftpServer clicked";

    if(sftpPool.size() < (getCurrentIndex() + 1))
        newSftpServer();

//    sftpPool.at(getCurrentIndex())->show();
}

void MainWindow::newSftpServer()
{
    //qDebug() << "newSftpServer clicked";

    if(paraPool.isEmpty())
    {
        //qDebug() << "未建立连接，请先新建连接";
        return;
    }

    if(tabPagePool.isEmpty())
        return;

    SftpServer *sftpServer = new SftpServer((*paraPool.at(getCurrentIndex())),
                                            getCurrentIndex(),0 ,tabPagePool.at(getCurrentIndex()), this);
    sftpPool.append(sftpServer);
}

void MainWindow::closeSftpServer()
{
    //qDebug() << "closeSftpServer clicked";
    if(sftpPool.size() < getCurrentIndex())
        return;

    sftpPool.at(getCurrentIndex())->close();
}

void MainWindow::openTreeView()
{
    //qDebug() << "MainWindow::openTreeView clicked";
    sftpPool.at(getCurrentIndex())->openTreeView();
}

int MainWindow::getCurrentIndex()
{
    return ui->tabWidget->currentIndex();
}

void MainWindow::errorHandle(int winNo, QString error)
{
    paraPool.remove(winNo);
    shellPool.remove(winNo);
    QMessageBox::warning(this,"连接出错",error,QMessageBox::Ok);
}

void MainWindow::openStyleSheet()
{
    QFile file(":/src/style.css");
    if(file.open(QFile::ReadOnly))
    {
        QString styleSheet = tr(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}

void MainWindow::closeTab(int index)
{
    if(ui->tabWidget->count() >= 1)
    {
        ui->tabWidget->removeTab(index);
        if(index < tabPagePool.size())
            tabPagePool.removeAt(index);
        if(index < sftpPool.size())
            sftpPool.removeAt(index);
        if(index < shellPool.size())
            shellPool.removeAt(index);
    }

    //选项卡全部关闭则关闭小企鹅
    if(ui->tabWidget->count() == 0)
        close();
}

void MainWindow::colorDeal(QString &mse)
{
    int i=0;
    int color;
    int background;
    QString startHtml1="<span style=\" background:";
    QString startHtml2=";\"><font color=";
    QString startHtml3=">";
    QString endHtml="</font></span>";
    QString result;

    while(i<mse.size())
    {

      if(mse.at(i)==0X1B)
      {
          if(mse.at(i+3)=='m')
          {
            mse.remove(i,4);

            continue;
          }
          else
          {
              background=(mse.at(i+2).toAscii()-('0'))*10+(mse.at(i+3).toAscii()-'0');
              qDebug()<<"sssssssss"<<mse.at(i+2);
              qDebug()<<"sssssssss"<<mse.at(i+3);
              color=(mse.at(i+5).toAscii()-'0')*10+(mse.at(i+6).toAscii()-'0');
              result=startHtml1+LinuxColor.value(background-10)+startHtml2+LinuxColor.value(color)+startHtml3;
              mse.remove(i,8);
              mse.insert(i,result);
              while(1)
              {
                  if(mse.at(i)==0X1B)
                  {
                      mse.remove(i,4);
                      break;
                  }
                  i++;
              }
              mse.insert(i,endHtml);
          }

      }
      if(mse.at(i)=='\r')
            {
                    mse.remove(i,2);
                    mse.insert(i,"<br/>");
            }
            i++;
          }
 }

