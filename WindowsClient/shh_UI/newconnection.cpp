#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnection)
{
    ui->setupUi(this);
}

NewConnection::~NewConnection()
{
    delete ui;
}
