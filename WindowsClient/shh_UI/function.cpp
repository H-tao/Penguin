#include "function.h"

QIcon getFileIcon(const QString name)
{
    QIcon icon(":/icons/folder.png");

    if(!name.isEmpty())
     {
      //获取后缀
      std::string strSuffix = "file";
      int index = name.lastIndexOf(".");
      if(index >= 0)
      {
          //将QString转化为std::string
          strSuffix = name.mid(index).toUtf8().constData();
      }

      SHFILEINFOA info;
      if(SHGetFileInfoA(strSuffix.c_str(),
                        FILE_ATTRIBUTE_NORMAL,
                        &info,
                        sizeof(info),
                        SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
      {
          HICON hicon = info.hIcon;
          icon = QIcon(QtWin::fromHICON(hicon));

      }
    }

    return icon;
}

QIcon getFolderIcon()
{
    return QFileIconProvider().icon(QFileIconProvider::Folder);
}
