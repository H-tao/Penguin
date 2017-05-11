#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QLabel>
#include<QDebug>
#include<FileSystemWidget/filewidget.h>

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

    void setTextEditSytel();
    void uiDesignerStyle();

private slots:
    //文件管理系统部分
    void on_openFileSystemAction_triggered();   //打开文件系统动作
    void on_closeFileSystemAction_triggered();  //关闭文件系统动作
    void on_showFileSystemAction_triggered();   //显示文件系统动作
    void on_concealFileSystemAction_triggered();    //隐藏文件系统动作
    void on_newConnectionAction_triggered();    //建立新连接动作

private:
    Ui::MainWindow *ui;

    //文件管理系统部分变量
    bool isOpenFileSystem = false;
    bool isShowFileSystem = false;
};

#endif // MAINWINDOW_H
