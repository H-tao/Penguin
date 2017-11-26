#include "tools.h"
Tools::Tools()
{

}

QString Tools::sizeToString(const qint64 &fileSize)
{
    quint64 size = quint64(fileSize);
    QString unit;
    QStringList units;
    units << "字节"<< "KB" << "MB" << "GB" << "TB";

    int i = 0;

    for(auto unit : units)
    {
        if(size < 1024.0)
            break;

        size /= 1024.0;
        ++i;
    }
    return QString::number(size) + QString(" ") + units.at(i);
}
