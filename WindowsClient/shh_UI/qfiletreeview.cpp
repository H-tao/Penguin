#include "qfiletreeview.h"
#include <QDebug>

QFileTreeView::QFileTreeView(QWidget *parent) :
    QTreeView(parent)
{

}


void QFileTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << this->currentIndex().row() << " " <<
                this->currentIndex().column() << " " << this->currentIndex().data();
}
