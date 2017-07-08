#ifndef TABPAGE_H
#define TABPAGE_H

#include <QWidget>
#include <FileSystemWidget/filewidget.h>
#include <ShellTextEdit/shelltextedit.h>

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
    void openFileSystem();   //打开文件系统
    void closeFileSystem();  //关闭文件系统
    void showFileSystem();   //显示文件系统
    void concealFileSystem();    //隐藏文件系统

    /**** textEdit、textLine、fileWidget 设置为Public，外部可以访问****/
    FileWidget *fileWidget;
    ShellTextEdit *textEdit;
    QLineEdit *filePathLineEdit;

    //文件资源管理系统部分变量
    bool isOpenFileSystem = false;
    bool isShowFileSystem = false;

private:
    Ui::TabPage *ui;
};

#endif // TABPAGE_H
