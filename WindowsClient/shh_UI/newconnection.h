#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>

namespace Ui {
class NewConnection;
}

class NewConnection : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnection(QWidget *parent = 0);
    ~NewConnection();
    QString getHost(){return host;}
    QString getuUserName(){return username;}
    QString getPassword(){return password;}
    QString getInformation(){return information;}
    int getPort(){return port;}
    void setText();
private:
    Ui::NewConnection *ui;
    QString host;
    QString username;
    QString password;
    int port;
    QString information;
};

#endif // NEWCONNECTION_H
