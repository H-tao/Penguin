#ifndef SHELLTEXTEDIT_H
#define SHELLTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

class ShellTextEdit : public QTextEdit
{
public:
    explicit ShellTextEdit(QWidget *parent);
    ~ShellTextEdit();
    
    void initSytle();   //初始化文本框风格
};

#endif // SHELLTEXTEDIT_H
