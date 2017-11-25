#include "textedit.h"
#include "ui_textedit.h"
#include<QDebug>
TextEdit::TextEdit(QWidget *parent,int number) :
    QMainWindow(parent),
    ui(new Ui::TextEdit),num(number)
{
    ui->setupUi(this);
    connect(ui->textEdit11,SIGNAL(textChanged()),this,SLOT(textChange()));
}

TextEdit::~TextEdit()
{
    fp->remove();
    delete ui;
}
void TextEdit::textChange()
{
    change=true;
}
void TextEdit::run(QString file,QString fileTypes)
{
    filename=file;
    this->setWindowTitle(filename);
    this->fileType=fileTypes;
    fp=new QFile(file);
    if(fp->open(QIODevice::ReadWrite|QIODevice::Text))
    {
        textStream=new QTextStream(fp);
       while(!textStream->atEnd())
       {
           ui->textEdit11->append(textStream->readLine());
       }
    }
    else
    {
        QMessageBox::warning(this,"File opem failure","Something wrong happening",QMessageBox::Yes);
    }
}

void TextEdit::on_action_triggered()
{
    bool ok;
    QFont font;
    font=QFontDialog::getFont(&ok);
    if(ok)
    ui->textEdit11->setFont(font);
}

void TextEdit::on_action_S_triggered()
{
    delete textStream;
    fp->close();
    fp->open(QIODevice::Truncate);
    fp->close();
    fp->open(QIODevice::WriteOnly);
    textStream=new QTextStream(fp);
    (*textStream)<<ui->textEdit11->toPlainText();
    textStream->flush();
    emit SaveOpenFile(filename,fileType,this->num);
}

void TextEdit::on_action_S_2_triggered()
{
    if(change)
    {
         if(QMessageBox::Yes==QMessageBox::warning(this,"警告","文件已已修改，是否保存?",QMessageBox::Yes,QMessageBox::No))
        {
             on_action_S_triggered();
        }
            else
           {
                exit(0);
           }
    }
    else
    {
      exit(0);
    }
}
