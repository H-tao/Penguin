#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class FileTreeView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FileTreeView(QWidget *parent = 0);
    ~FileTreeView();

    QTreeWidgetItem* addTreeRoot(QString fileName, QIcon icon);
    QTreeWidgetItem* addTreeNode(QTreeWidgetItem *parent, QString fileName, QIcon icon);

};

#endif // FILETREEVIEW_H
