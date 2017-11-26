#include "tabpage.h"
#include "ui_tabpage.h"

TabPage::TabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPage)
{
    ui->setupUi(this);

    initStyle();
    isShowFileSystem = false;
    isOpenFileSystem = false;
}

TabPage::~TabPage()
{
    delete ui;
}

void TabPage::initStyle()
{
    /****** 用public指针指向私有成员，外部访问可通使用指针 *******/
    textEdit = new ShellTextEdit(ui->textWidget);   //新增TextEdit
    ui->textWidgetLayout->addWidget(textEdit);

    filePathLineEdit = ui->pathCombo;    //新增LineEdit
    filePathLineEdit->setEditable(true);
    filePathLineEdit->setEnabled(false);
    searchEdit = ui->searchCombo;
    searchEdit->setEditable(true);
    searchEdit->setEnabled(false);

//    fileWidget = new FileWidget(ui->fileSystemWidget);  //新增FileWidget
    fileWidget = new FileTreeView(ui->fileSystemWidget);
    ui->fileSystemWidgetLayout->addWidget(fileWidget);
//    multiDownloadWidget = new DownloadWidget(ui->multiDownloadFrame);
//    ui->multiDownloadLayout->addWidget(multiDownloadWidget);
    multiDownloadLayout = new QVBoxLayout(ui->multiDownloadFrame);
    multiDownloadLayout->setMargin(0);
    multiDownloadLayout->setSpacing(0);
    ui->multiDownloadFrame->setLayout(multiDownloadLayout);

    maxiBtn = ui->maximizeBtn;
    miniBtn = ui->minimizeBtn;

    //设置初始用于显示文件资源管理系统的窗口为不可见
    ui->fileSystemAndMultiDownload->setVisible(false);
//    ui->fileSystemWidget->setVisible(false);
//    ui->multiDownloadFrame->setVisible(false);

//    filePathLineEdit->setFocusPolicy(Qt::NoFocus);
}

//打开文件管理系统
void TabPage::openFileSystem()
{
   // qDebug() << "TabPage::openFileSystem";
    //如果文件管理系统已经打开
    if(isOpenFileSystem)
    {
        showFileSystem();
        return;
    }

    fileWidget->openFileSystem();                                              //TODO 用于测试是否能显示图标
    ui->fileSystemWidget->setFrameShape(QFrame::NoFrame);
    showFileSystem();
    isOpenFileSystem = true;
    // 初始化ComboBox
    filePathLineEdit->setEditText("");
    filePathLineEdit->setEnabled(true);
    searchEdit->setEnabled(true);
}

void TabPage::closeFileSystem()
{
    //此处为方便下一次打开做隐藏处理
    concealFileSystem();
}

void TabPage::showFileSystem()
{
    if(isShowFileSystem == false)
    {
       ui->fileSystemAndMultiDownload->setVisible(true);
       //设置显示文件管理系统的值为真
       isShowFileSystem = true;
       return;
    }
    //如果已经文件管理系统打开
/*
    if(isShowFileSystem == true)
    {
        //提示对话框
        QMessageBox::information(this, tr("提示"), tr("文件管理系统已经显示！"), QMessageBox::Ok);
    }*/
}

void TabPage::concealFileSystem()
{
    if(isShowFileSystem == false)
        return;

    if(isShowFileSystem == true)
    {
        ui->fileSystemAndMultiDownload->setVisible(false);
        isShowFileSystem = false;
        return;
    }
}

