#ifndef FILESYSTEMWIDGET_H
#define FILESYSTEMWIDGET_H

#include <QWidget>
#include<QFileInfo>
#include<QVector>
#include<QLabel>
#include<QImage>
#include<QHBoxLayout>
#include<QGridLayout>
#include<QResizeEvent>
#include<EnhancedLabel/enhancedlabel.h>

namespace Ui {
class FileSystemWidget;
}

class FileSystemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemWidget(QWidget *parent = 0);
    ~FileSystemWidget();

    void openFileSystem(QFileInfoList fileInfoList);
    QString findIconPath(QString FileSuffix);
    void setGeometryOfImageLabel();

protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::FileSystemWidget *ui;
    QVector<QWidget*> fileWidgets;
    QVector<QLabel*> imageLabels;        //用于显示图片的Label
    QWidget *fileSystemWidget;

    //QFileInfoList 用于测试
    QFileInfoList testFileInfoList;

    QVector<EnhancedLabel *> fileLabels;
};

#endif // FILESYSTEMWIDGET_H
