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
#include<QTextCursor>
class ShellTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit ShellTextEdit(QWidget *parent = 0);
    ~ShellTextEdit();
    void initSytle();   //初始化文本框风格
    void clearTheKeyin();
signals:
    void arguementDone(QString);

protected:
    void keyPressEvent(QKeyEvent *e);
 //   void mousePressEvent(QMouseEvent *e);
 //   void mouseReleaseEvent(QMouseEvent *e);

private:
    QString arguement;
    QString console;
    QVector<QString> history;
    int hisNum;
    bool enterHistory;
};

#endif // SHELLTEXTEDIT_H
