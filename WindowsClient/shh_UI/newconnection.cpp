#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnection)
{
    ui->setupUi(this);
     host=ui->hostNumLineEdit->text();
   name=ui->nameLineEdit->text();
     protocol=ui->protocolLineEdit->text();
     port=(int)ui->portNumSpinBox->value();
     information=ui->statusTextEdit->toPlainText();
}

NewConnection::~NewConnection()
{
    delete ui;
}
