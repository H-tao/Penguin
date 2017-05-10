#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QIcon>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QString>
#include <QDebug>

namespace Ui {
class FileSystem;
}

class FileSystem : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystem(QWidget *parent = 0);
    ~FileSystem();
    FileSystem(QListWidget *fileWidget, int a);
    void openFileSystem();
    void setFileInfoList(QFileInfoList infoList);
    QIcon getFileIcon(const QString &suffix) const;
    void PrintLW();
    void Working();

private:
    Ui::FileSystem *ui;
    QFileInfoList fileInfoList;
    QListWidget *listWidget;

};

#endif // FILESYSTEM_H
