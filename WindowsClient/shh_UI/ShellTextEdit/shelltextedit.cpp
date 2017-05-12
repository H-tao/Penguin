#include "shelltextedit.h"

ShellTextEdit::ShellTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    initSytle();
}

ShellTextEdit::~ShellTextEdit()
{
    
}

void ShellTextEdit::initSytle()
{
    //设置文本标签背景
    setStyleSheet("background-color:#242424");
    //设置字体颜色
    QPalette pal = palette();
    //pal.setColor(QPalette::Background, QColor(0,0,0));
    pal.setColor(QPalette::Text, QColor(250,250,250));
    setPalette(pal);
    //设置初始化文本
    setText("Welcome to use Penguin Terminal Emulator.\n\n"
                          "Initializing...\n\n"
                          "Penguin Terminal Emulator Initialization Completed.\n\n"
                          "Connecting to localhost...\n"
                          "[d:\\~]$ ");
}
