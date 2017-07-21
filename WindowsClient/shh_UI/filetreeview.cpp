#include "filetreeview.h"

FileTreeView::FileTreeView(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(1);
    this->setHeaderHidden(true);
}

FileTreeView::~FileTreeView()
{

}


QTreeWidgetItem* FileTreeView::addTreeRoot(QString fileName, QIcon icon)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << fileName);
    item->setIcon(0, icon);
    this->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem* FileTreeView::addTreeNode(QTreeWidgetItem *parent, QString fileName, QIcon icon)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << fileName);
    item->setIcon(0, icon);
    parent->addChild(item);
    return item;
}
