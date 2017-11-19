#include "shelltextedit.h"
#include<QDataStream>
ShellTextEdit::ShellTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    initSytle();
    arguement = "";
    console="";
//    setReadOnly(true);
    enterHistory=false;
    lastHistory=false;
}

ShellTextEdit::~ShellTextEdit()
{

}

void ShellTextEdit::initSytle()
{
    //设置文本标签背景
    setStyleSheet("background-color:#484848");
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
    //输入命令时设置光标位置为文本末尾
    QTextCursor textCursor = this->textCursor();
    textCursor.movePosition(QTextCursor::End);
    this->setTextCursor(textCursor);
    if(e->key()==Qt::Key_Return)
    {
        if(arguement.isEmpty()) //命令为空，返回
        {
            arguement+="\n";
            emit arguementDone(arguement);
            arguement = "";
            return;
        }
        history.append(arguement);
        arguement+="\n";
        enterHistory=true;
        hisNum=history.size();
        emit arguementDone(arguement);
        arguement = "";
        return;
    }
    //Control
    else if(e->modifiers() == Qt::ControlModifier&&e->key()>64&&e->key()<95)
    {
        char chara=e->key()-64;
        arguement+=chara;
        return;
    }
    //方向键
    else if(e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        if(e->key()==Qt::Key_Right||e->key()==Qt::Key_Left)
            QTextEdit::keyPressEvent(e);
        else if(e->key()==Qt::Key_Up)
        {
            if(enterHistory)
            {
                if(hisNum==0)
                    return;
                hisNum-=1;
                lastHistory=true;
                clearTheKeyin();
                arguement=history.at(hisNum);
            }
            else
            {
                return;
            }


        }
       else if(e->key()==Qt::Key_Down)
        {
            if(enterHistory)
            {
                if(hisNum==history.size()-1)
                {
                    lastHistory=false;
                    return;
                }
                hisNum+=1;
                lastHistory=true;
                clearTheKeyin();
                arguement=history.at(hisNum);
            }
            else
            {
                lastHistory=false;
                return;
            }

        }
        insertPlainText(arguement);
        return;
    }

    if(e->key() <= 31 && e->key() >= 0)     //ASCII 控制字符暂不做命令处理
    {
        QTextEdit::keyPressEvent(e);
    }
    if(e->key() <= 256 && e->key() >= 32)   //非控制字符
    {
        arguement += e->text();
      QTextEdit::keyPressEvent(e);
    }
    if(e->key()==Qt::Key_Backspace/*e->text() == "\b"*/)         //退格键删除
    {

        if(arguement.size() == 0)
        {
           e->ignore();
           return;
        }
        if(arguement.at(arguement.size()-1).toAscii()<=28)
           { QTextEdit::keyPressEvent(e);
            //qDebug()<<arguement.at(arguement.size()-1).toAscii();
        }
        QTextEdit::keyPressEvent(e);
        arguement.remove(arguement.size()-1,1);
    }
    if(e->key()==Qt::Key_Escape)
    {
        qDebug()<<"Esc键键入";
        QByteArray t;
        char a=0X1B;
        t.append(a);
        emit arguementDone(t);
        return;
    }
    else if(e->key()==Qt::Key_Insert)
    {
       QByteArray t;
       char a[4]={0X1B,0X58,0X32,0X7E};
       t.append(a,4);
        emit arguementDone(t);
       return;
    }
}

void ShellTextEdit::clearTheKeyin()
{
    int i=arguement.size();
    QKeyEvent *e;
    e=new QKeyEvent(QEvent::Type::KeyPress,16777219,Qt::KeyboardModifier::NoModifier);
    while(i--)
    {
        this->QTextEdit::keyPressEvent(e);
    }
    arguement="";
}
