#ifndef QFILETREEVIEW_H
#define QFILETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QMouseEvent>

class QFileTreeView : public QTreeView
{
public:
    explicit QFileTreeView(QWidget *parent = 0);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

};

#endif // QFILETREEVIEW_H
