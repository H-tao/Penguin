#include "shelltextedit.h"
ShellTextEdit::ShellTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    initSytle();
    arguement = "";
    console="";
//    setReadOnly(true);
    ptr=history.end();
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
    //输入命令时设置光标位置为文本末尾
    QTextCursor textCursor = this->textCursor();
    textCursor.movePosition(QTextCursor::End);
    this->setTextCursor(textCursor);
    if(/*e->key()==Qt::Key_Enter||*/e->key()==Qt::Key_Return)
    {
        qDebug() << "Enter Done";
        history.append(arguement);
        ptr=history.end();
        emit arguementDone(arguement);
        arguement = "";
        return;
    }
    //Control
    else if(e->modifiers() == Qt::ControlModifier&&e->key()>64&&e->key()<95)
    {
        char chara=e->key()-64;
        arguement+=chara;
        this->moveCursor(QTextCursor::EndOfLine);
        insertPlainText("^"+QChar(e->key()));
        return;
    }
    //Alt
    else if(e->modifiers() == Qt::AltModifier)
    {  QTextEdit::keyPressEvent(e);
        return;
    }
    //方向键
    else if(e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        if(e->key()==Qt::Key_Up)
        {
            if(ptr==history.end())
            {
                ptr-=2;
            }
            else
            {
                ptr--;
            }
            QTextCursor txtcur=this->textCursor();
             txtcur.setPosition(0);
             txtcur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
             txtcur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor,-1*arguement.size());
             txtcur.removeSelectedText();
             qDebug()<<txtcur.selectedText();
             insertPlainText(*ptr);
             arguement=*ptr;
             return;
        }
        if(e->key()==Qt::Key_Down)
        {
            if(ptr==history.end())
                ptr=history.begin();
            else
                ptr++;
            QTextCursor txtcur= this->textCursor();
             txtcur.setPosition(0);
             txtcur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
             txtcur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor,-1*arguement.size());
             txtcur.removeSelectedText();
             insertPlainText(*ptr);
             arguement=*ptr;
             return;
        }
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
        //命令为空，不可删除，即为ReadOnly
        if(arguement.size() == 0)
        {
           e->ignore();
           return;
        }
        if(arguement.at(arguement.size()-1)<='\0X1F')
            QTextEdit::keyPressEvent(e);
        QTextEdit::keyPressEvent(e);
        arguement.remove(arguement.size()-1,1);
    }

    if(e->key()<='\0x01000060'&&e->key()>='\0X01000000')
    {
        console+=e->key();
        emit arguementDone(console);
        console="";
    }
    /***** Esc ******/
/*
    if(e->key() == 16777222)
        arguement = "\u007F";
    if(e->key() == 16777216)
        arguement = "\u001B";

    QTextEdit::keyPressEvent(e);    //TODO 极重要，将输入显示到命令行

 /*   if(e->modifiers()==Qt::ControlModifier&&e->key()>64&&e->key()<95)
    {
        keyReleaseEvent(e);
        arguement+=(QChar(e->key()-64));
        this->moveCursor(QTextCursor::EndOfLine);
        insertPlainText("^"+QChar(e->key()));
        i+=2;
    }
    else if(e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return)
    {
        qDebug() << "Enter Done";
        emit arguementDone(arguement);
        arguement = "";
        qDebug() << "After \\r arguement：" << arguement;
        return;
    }
    else if(e->key()==Qt::Key_Backspace)    //退格键删除
    {
        if(i>0)
        {
            setReadOnly(false);
            this->QTextEdit::keyPressEvent(e);
             arguement.remove(arguement.size() - 1, 1);     //最后一个char位于size()-1
             qDebug()<<arguement.at(arguement.size()-1);
            i--;
            if(arguement.at(arguement.size()-1)<='\31'){
                qDebug()<<"2222222";
                arguement.remove(arguement.size(),1);
                this->QTextEdit::keyPressEvent(e);
                i--;
            }

            setReadOnly(true);
        }
        else
            return;
    }
    // Control
    else if(e->modifiers()!=Qt::ControlModifier)
    {
        setReadOnly(false);
        this->QTextEdit::keyPressEvent(e);
        arguement.append(QChar(e->key()));
        i++;
        setReadOnly(true);
    }
    // Alt
    else if(e->modifiers() == Qt::AltModifier)
    {
        qDebug() << "Alt!!!";
        return;
    }*/
}
/*
void ShellTextEdit::mousePressEvent(QMouseEvent *e)
{

}*/
/*
void ShellTextEdit::mouseReleaseEvent(QMouseEvent *e)
{

}*/
