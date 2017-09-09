/*************************************************************************
 * 每一个标签页中都包含
 * 1、TextEdit——用于发送命令和接受从远端传回的信息
 * 2、TextLine——用于显示文件管理系统的当前路径（后期可以进行修改）
 * 3、FileWidget——文件管理系统，用于显示当前目录下的所有文件，并可进行操作
**************************************************************************/
#ifndef TABPAGE_H
#define TABPAGE_H

#include <QWidget>
#include <FileSystemWidget/filewidget.h>
#include <ShellTextEdit/shelltextedit.h>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class TabPage;
}

class TabPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabPage(QWidget *parent = 0);
    ~TabPage();

    void initStyle();   //初始化标签页风格

    /**** textEdit、textLine、fileWidget 设置为Public，外部可以访问****/
    FileWidget *fileWidget;
    ShellTextEdit *textEdit;
    QComboBox *filePathLineEdit;
    QPushButton *maxiBtn;
    QPushButton *miniBtn;

    //文件资源管理系统部分变量
    bool isOpenFileSystem = false;
    bool isShowFileSystem = false;

public slots:
    void openFileSystem();   //打开文件系统
    void closeFileSystem();  //关闭文件系统
    void showFileSystem();   //显示文件系统
    void concealFileSystem();    //隐藏文件系统

private:
    Ui::TabPage *ui;
};

#endif // TABPAGE_H
