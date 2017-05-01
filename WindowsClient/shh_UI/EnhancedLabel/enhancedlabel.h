#ifndef ENHANCEDLABEL_H
#define ENHANCEDLABEL_H

#include <QWidget>
#include<QMouseEvent>
#include<QEvent>
#include<QLabel>
#include<QString>

namespace Ui {
class EnhancedLabel;
}

class EnhancedLabel : public QWidget
{
    Q_OBJECT

public:
    explicit EnhancedLabel(QWidget *parent = 0);
    void set(const QString imagePath, const QString fileName);
    ~EnhancedLabel();

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::EnhancedLabel *ui;
    bool checked;
};

#endif // ENHANCEDLABEL_H
