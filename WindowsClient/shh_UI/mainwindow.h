#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QLabel>
#include<QDebug>
#include<FileSystemWidget/filewidget.h>
#include<ShellTextEdit/shelltextedit.h>
#include<ssh/sshconnection.h>
#include<newconnection.h>

class ShellTextEdit;
class FileWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initStyle();
    void setUiDesignerStyle();
    void connectAction();

private slots:
    //文件资源管理系统部分槽
    void on_openFileSystemAction_triggered();   //打开文件系统动作
    void on_closeFileSystemAction_triggered();  //关闭文件系统动作
    void on_showFileSystemAction_triggered();   //显示文件系统动作
    void on_concealFileSystemAction_triggered();    //隐藏文件系统动作
    void on_newConnectionAction_triggered();    //建立新连接动作
    void on_addTabAction_triggered();   //增加新选项卡动作
    void on_closeCurrentTabAction_triggered();  //关闭当前选项卡动作
//ssh部分
    void outToShell(int winNo,QString);//


private:
    Ui::MainWindow *ui;

    //文件资源管理系统部分变量
    bool isOpenFileSystem = false;
    bool isShowFileSystem = false;

    ShellTextEdit *textEdit;


    //ssh连接
    QSsh::SshConnectionParameters *sshPara;
    QSsh::SshConnection *sshCon;
    QSsh::SshRemoteProcess *shell;
};

#endif // MAINWINDOW_H
