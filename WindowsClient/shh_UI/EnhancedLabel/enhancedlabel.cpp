#include "enhancedlabel.h"
#include "ui_enhancedlabel.h"

EnhancedLabel::EnhancedLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnhancedLabel)
{
    ui->setupUi(this);
    checked = false;

}

EnhancedLabel::~EnhancedLabel()
{
    delete ui;
}

void EnhancedLabel::set(const QString imagePath, const QString fileName)
{
    int spacing = 20;
    QImage *image = new QImage;
    image->load(imagePath);
    ui->imgLabel->setPixmap(QPixmap::fromImage(*image));
    ui->imgLabel->setGeometry(0,0,image->width()+spacing,image->height());
    ui->imgLabel->setAlignment(Qt::AlignCenter);

    int nameLabelWidth = 20;
    int w = ui->imgLabel->width();
    int h = ui->imgLabel->height();
    ui->fileNameLabel->setText(fileName);
    ui->fileNameLabel->setGeometry(0,h,w,nameLabelWidth);
//    ui->fileNameLabel->setAlignment(Qt::AlignRight);
    resize(w,h+nameLabelWidth);
}

void EnhancedLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit doubleClicked();
}

void EnhancedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        ui->imgLabel->setStyleSheet("QLabel{background:#CCE8FF;border:1px solid blue;border-bottom:transparent}");
        ui->fileNameLabel->setStyleSheet("QLabel{background:#CCE8FF;border:1px solid blue;border-top:transparent}");
    }
    if(event->button() == Qt::RightButton)
        return;
}

void EnhancedLabel::enterEvent(QEvent *)
{
    ui->imgLabel->setStyleSheet("QLabel{background:#E5F3FF;border:transparent}");
    ui->fileNameLabel->setStyleSheet("QLabel{background:#E5F3FF;border:transparent}");
}

void EnhancedLabel::leaveEvent(QEvent *)
{
    ui->imgLabel->setStyleSheet("");
    ui->fileNameLabel->setStyleSheet("");
}
