#ifndef FILENAMEDIALOG_H
#define FILENAMEDIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>

namespace Ui {
class FileNameDialog;
}

class FileNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileNameDialog(QWidget *parent = 0);
    ~FileNameDialog();

    void setText(QString str);
    QString getText();
    bool isIllegalChar(QChar ch);

public slots:
    void textEditChanged(QString str);
    void okClicked();

private:
    Ui::FileNameDialog *ui;
    QLineEdit *m_edit;

};

#endif // FILENAMEDIALOG_H
