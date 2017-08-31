#include "filenamedialog.h"
#include "ui_filenamedialog.h"
#include <QVBoxLayout>
#include <QToolTip>

FileNameDialog::FileNameDialog(QWidget *parent) :
    QDialog(parent)
//    ,ui(new Ui::FileNameDialog)
{
//    ui->setupUi(this);

    m_edit = new QLineEdit(this);
    connect(m_edit, SIGNAL(textEdited(QString)), this, SLOT(textEditChanged(QString)));
    connect(m_edit, SIGNAL(returnPressed()), this, SLOT(okClicked()));

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);
    this->setFixedSize(200,40);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->addWidget(m_edit);
}

FileNameDialog::~FileNameDialog()
{
//    delete ui;
}

void FileNameDialog::setText(QString str)
{
    m_edit->setText(str);
    m_edit->setSelection(0,str.length());
}

QString FileNameDialog::getText()
{
    return m_edit->text();
}

void FileNameDialog::textEditChanged(QString str)
{
    // the text can't include  \/:*?“<>|
    for(int i = 0; i < str.length(); ++i)
    {
        if(isIllegalChar(str.at(i)))
        {
            str.remove("[\\/:*?\“<>|]");
            m_edit->setText(str);
            QToolTip::showText(mapToGlobal(m_edit->pos()),
                               tr("Can't include \\/:*?\"<>|"));
            return;
        }
    }
}

bool FileNameDialog::isIllegalChar(QChar ch)
{
    if(ch == '\\' || ch == '/' || ch == ':' ||
            ch == '*' || ch == '?' || ch == '"' ||
            ch == '<' || ch == '>' || ch == '|')
        return true;
    else
        return false;
}

void FileNameDialog::okClicked()
{
    if(m_edit->text().isEmpty())
    {
        QToolTip::showText(mapToGlobal(m_edit->pos()),
                           tr("The file's name can't be empty!"));
        return;
    }

    emit accept();
}
