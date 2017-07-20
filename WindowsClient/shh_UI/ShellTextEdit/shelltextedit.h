#ifndef SHELLTEXTEDIT_H
#define SHELLTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QKeyEvent>
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QEvent>

class ShellTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit ShellTextEdit(QWidget *parent);
    ~ShellTextEdit();
int i;
    void initSytle();   //初始化文本框风格
signals:
    void arguementDone(QString);

protected:
    void keyPressEvent(QKeyEvent *e);
 //   void mousePressEvent(QMouseEvent *e);
 //   void mouseReleaseEvent(QMouseEvent *e);

private:
    QString arguement;
};

#endif // SHELLTEXTEDIT_H
