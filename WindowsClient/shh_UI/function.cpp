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

QString getPermissions(QFile::Permissions permissions)
{
    QString str("");

    if(QFile::ReadUser == (QFile::ReadUser & permissions))
    {
        str +="r";
    }
    else
    {
        str +="-";
    }
    if(QFile::WriteUser == (QFile::WriteUser & permissions))
    {
        str +="w";
    }
    else
    {
        str +="-";
    }
    if(QFile::ExeUser == (QFile::ExeUser & permissions))
    {
        str +="x";
    }
    else
    {
        str +="-";
    }

    if(QFile::ReadGroup == (QFile::ReadGroup & permissions))
    {
        str +="r";
    }
    else
    {
        str +="-";
    }
    if(QFile::WriteGroup == (QFile::WriteGroup & permissions))
    {
        str +="w";
    }
    else
    {
        str +="-";
    }
    if(QFile::ExeGroup == (QFile::ExeGroup & permissions))
    {
        str +="x";
    }
    else
    {
        str +="-";
    }

    if(QFile::ReadOther == (QFile::ReadOther & permissions))
    {
        str +="r";
    }
    else
    {
        str +="-";
    }
    if(QFile::WriteOther == (QFile::WriteOther & permissions))
    {
        str +="w";
    }
    else
    {
        str +="-";
    }
    if(QFile::ExeOther == (QFile::ExeOther & permissions))
    {
        str +="x";
    }
    else
    {
        str +="-";
    }

    return str;
}

QString getSizeFromByte(quint64 iByte)
{

    if(iByte < 1000)
    {
        return QString("%1 B").arg(iByte);
    }

    double fKB = iByte/1024.0;
    if(fKB < 1000)
    {
        return QString("%1 KB").arg(QString::number(fKB, 'g', 3));
    }

    double fMB = fKB/1024.0;
    if(fMB <1000)
    {
        return QString("%1 MB").arg(QString::number(fMB, 'g', 3));
    }

    double fGB = fMB/1024.0;
    return QString("%1 GB").arg(QString::number(fGB, 'g', 3));

}

quint64 getSizeToByte(QString strSize)
{
    quint64 uByte=0;

    // get unit
    int iPos = strSize.lastIndexOf(" ");
    QString strUnit = strSize.right(strSize.length()-iPos-1);
    QString strNum = strSize.left(iPos);
    double dNum = strNum.toDouble();

    // transform
    if("GB" == strUnit)
    {
        uByte = (quint64)(dNum*1024*1024*1024);
    }
    else if("MB" == strUnit)
    {
        uByte = (quint64)(dNum*1024*1024);
    }
    else if("KB" == strUnit)
    {
        uByte = (quint64)(dNum*1024);
    }
    else
    {
        uByte = (quint64)(dNum);
    }


    return uByte;
}

QString getFileType(QString strName)
{
    QString strType = QString("unknown");

    if(!strName.isEmpty())
    {
        //get suffix
        std::string strSuffix = "file";
        int index = strName.lastIndexOf(".");
        if(index >= 0)
        {
            //QString to std::string
            strSuffix = strName.mid(index).toUtf8().constData();
        }

        SHFILEINFOA info;
        if(SHGetFileInfoA(strSuffix.c_str(),
                         FILE_ATTRIBUTE_NORMAL,
                         &info,
                         sizeof(info),
                         SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
        {

            strType = QString::fromLocal8Bit(info.szTypeName);
        }
    }

    return strType;
}

QString getFolderType()
{
    QString strType = QString("Folder");

    SHFILEINFOA info;
    if(SHGetFileInfoA("folder",
                     FILE_ATTRIBUTE_DIRECTORY,
                     &info,
                     sizeof(info),
                     SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
    {

        strType = QString::fromLocal8Bit(info.szTypeName);
    }

    return strType;
}
