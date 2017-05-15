#ifndef CONWIDEGT_H
#define CONWIDEGT_H

#include <QDialog>
#include"ui_new.h"
#include<QString>
namespace Ui {
class ConWidegt;
}

class ConWidegt : public QDialog
{
    Q_OBJECT

public:
    explicit ConWidegt(QWidget *parent = 0);
    ~ConWidegt();
    QString get_name(){return name;}
    QString get_protocol(){return protocol;}
    QString get_host(){return host;}
    QString get_status(){return status;}
    int get_port(){return port;}
    bool get_flag(){return flag;}
private slots:
    void on_buttonBox_accepted();

private:
    Ui::newDialog *ui;
    //连接属性
    QString name;
    QString protocol;
    QString host;
    int port;
    QString status;
    bool flag;

};

#endif // CONWIDEGT_H
