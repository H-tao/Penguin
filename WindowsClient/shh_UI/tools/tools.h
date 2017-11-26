#pragma once
#pragma execution_character_set("utf-8")

#include <QVector>

class Tools
{
public:
    Tools();
    static QString sizeToString(const qint64 &fileSize);
};

