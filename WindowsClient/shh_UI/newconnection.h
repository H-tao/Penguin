#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

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
    QString getPrivateKey(){return key;}
    int getPort(){return port;}
    void setText();
    void passwordConnect();
    void privateKeyConnect();
    bool isPasswordConnect();

public slots:
    void openKeyFile();
    void connectWayChanged();

private:
    Ui::NewConnection *ui;
    QString host;
    QString username;
    QString password;
    int port;
    QString key;
    QString information;
};

#endif // NEWCONNECTION_H
