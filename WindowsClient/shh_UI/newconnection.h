#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>

namespace Ui {
class NewConnection;
}

class NewConnection : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnection(QWidget *parent = 0);
    ~NewConnection();

private:
    Ui::NewConnection *ui;
};

#endif // NEWCONNECTION_H
