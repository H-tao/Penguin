#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnection)
{
    ui->setupUi(this);

}
void NewConnection::setText()
{
    host=ui->hostNumLineEdit->text();
  username=ui->nameLineEdit->text();
    password=ui->protocolLineEdit->text();
    port=(int)ui->portNumSpinBox->value();
    information=ui->statusTextEdit->toPlainText();
}
NewConnection::~NewConnection()
{
    delete ui;
}
