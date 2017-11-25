#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QMessageBox>
#include<QFontDialog>
#include<QByteArray>
#include<QMessageBox>
#include <QFileInfo>

namespace Ui {
class TextEdit;
}

class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent = 0,int num=0);
    ~TextEdit();
    void run(QString fileName,QString fileType);
private:
    Ui::TextEdit *ui;
    QString filename;
    bool change;
    QFile *fp;
    QString fileType;
    int num;
    QTextStream *textStream;
signals:
    SaveOpenFile(QString file,QString fileType,int num);
private slots:
    void textChange();
    void on_action_triggered();
    void on_action_S_triggered();
    void on_action_S_2_triggered();
};

#endif // TEXTEDIT_H
