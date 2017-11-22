#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnection)
{
    ui->setupUi(this);

    ui->connectionWayComboBox->addItem("密码");
    ui->connectionWayComboBox->addItem("公钥");
    ui->connectionWayComboBox->setEditable(false);

    // 默认密码连接
    ui->keyPasswordLineEdit->setEnabled(false);
    ui->keyPathLineEdit->setEnabled(false);
    ui->openFileBtn->setEnabled(false);

    connect(ui->openFileBtn, &QPushButton::clicked, this, &NewConnection::openKeyFile);
    connect(ui->connectionWayComboBox, &QComboBox::currentTextChanged, this, &NewConnection::connectWayChanged);
}

void NewConnection::setText()
{

}


NewConnection::~NewConnection()
{
    delete ui;
}

bool NewConnection::isPasswordConnect()
{
    if(ui->connectionWayComboBox->currentText() == "密码")
    {
        passwordConnect();
        return true;
    }
    else
    {
        privateKeyConnect();
        return false;
    }
}

void NewConnection::passwordConnect()
{
    host=ui->hostNumLineEdit->text();
    username=ui->nameLineEdit->text();
    password=ui->passwordLineEdit->text();
    port=(int)ui->portNumSpinBox->value();
    information=ui->statusTextEdit->toPlainText();
}

void NewConnection::privateKeyConnect()
{
    host=ui->hostNumLineEdit->text();
    username=ui->nameLineEdit->text();
    port=(int)ui->portNumSpinBox->value();
    information=ui->statusTextEdit->toPlainText();
}


void NewConnection::openKeyFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"),
                                                   "C:\\", tr("All (*.*)"));
    if(fileName.isEmpty())
        return;

    key = fileName;

    // 设置路径名
    ui->keyPathLineEdit->setText(fileName);
    qDebug() << "key : ";
    qDebug() << key;
}

void NewConnection::connectWayChanged()
{
    if(ui->connectionWayComboBox->currentText() == "密码")
    {
        ui->keyPasswordLineEdit->setEnabled(false);
        ui->keyPathLineEdit->setEnabled(false);
        ui->openFileBtn->setEnabled(false);
    }
    else
    {
        ui->keyPasswordLineEdit->setEnabled(true);
        ui->keyPathLineEdit->setEnabled(true);
        ui->openFileBtn->setEnabled(true);
        ui->passwordLineEdit->setEnabled(false);
    }
}
