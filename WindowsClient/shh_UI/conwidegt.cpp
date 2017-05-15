#include "conwidegt.h"
#include<QMessageBox>
bool hostCheck(QString host);
ConWidegt::ConWidegt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newDialog)
{
    ui->setupUi(this);
    flag=0;
}

ConWidegt::~ConWidegt()
{
    delete ui;
}

void ConWidegt::on_buttonBox_accepted()
{
    host=ui->hostNumLineEdit;
    if(!hostCheck(host))
    {
        QMessageBox::warning(this,"ERROR","THE HOST IS ERROR,PLEASE CHECK IT AGAIN!",QMessageBox::Button::Yes);
    }
    else
    {
        port=(int)ui->portNumSpinBox->value();
        if(port<0)
            QMessageBox::warning(this,"ERROR","THE PORT IS ERROR,PLEASE CHECK IT AGAIN!",QMessageBox::Button::Yes);
        protocol=ui->protocolLineEdit->text();
        if(ui->nameLineEdit->text()=="")
            name="nullname";
        else
            name=ui->nameLineEdit->text();
        status=ui->statusTextEdit->toPlainText();
        flag=1;
    }
}
bool hostCheck(QString host)
{
    int a[4]={0,0,0,0},t=0;

    for(auto i=host.begin();i!=host.end();i++)
    {
        if(i->toLatin1()=='.')
            t++;
        else
        {
            if(i->toLatin1()>'9'||i->toLatin1()<'0')
                return false;
            a[t]*=10;
            a[t]+=i->toLatin1()-'0';
        }
    }
    for(t=0;t<4;t++)
    {
        if(a[t]>256||a[t]<0)
            return false;
    }
    else
        return true;
}
