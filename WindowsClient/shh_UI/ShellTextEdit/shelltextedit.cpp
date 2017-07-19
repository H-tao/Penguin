#include "shelltextedit.h"

ShellTextEdit::ShellTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    initSytle();
    arguement = "";
    i=0;
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
                          "Connecting to ...\n"
                          /*"[d:\\~]$ "*/);
    //设置换行模式
     setWordWrapMode(QTextOption::NoWrap);
//   setReadOnly(true);
}

/*****************************************************************************************
 * http://www.51hei.com/mcu/4342.html
 * ASCII大致由三部分组成：
 * 1、ASCII 打印字符：数字 32–126 分配给了能在键盘上找到的字符，当您查看或打印文档时就会出现。注：十进制32代表空格 ，十进制数字 127 代表 DELETE 命令。
 * 2、ASCII 非打印控制字符：ASCII 表上的数字 0–31 分配给了控制字符，用于控制像打印机等一些外围设备。例如，12 代表换页/新页功能。此命令指示打印机跳到下一页的开头。
 * 3、扩展 ASCII 打印字符：扩展的 ASCII 字符满足了对更多字符的需求。扩展的 ASCII 包含 ASCII 中已有的 128 个字符（数字 0–32 显示在下图中），又增加了 128 个字符，总共是 256 个。即使有了这些更多的字符，许多语言还是包含无法压缩到 256 个字符中的符号。因此，出现了一些 ASCII 的变体来囊括地区性字符和符号.
 * ****************************************************************************************/
void ShellTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier)
    {
        keyReleaseEvent(e);
        arguement.append(QChar(e->key()-64));
        i+=2;
    }
    else if(e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return)
    {
       emit arguementDone(arguement);
    }
    else if(e->key()==Qt::Key_Backspace)
    {
        if(i>0)
        {
            setReadOnly(false);
            this->QTextEdit::keyPressEvent(e);
            i--;
            if(arguement.end()->toAscii()<=31){
                arguement.remove(arguement.size()-1,2);
                this->QTextEdit::keyPressEvent(e);
                i--;
            }

            setReadOnly(true);
            //
        }
        else
            return;
    }
    else
    {
        setReadOnly(false);
        this->QTextEdit::keyPressEvent(e);
        arguement.append(QChar(e->key()));
        i++;
        setReadOnly(true);
    }
}
