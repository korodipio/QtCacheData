#include "cachedata.h"

#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CacheData cache("seed");
    qDebug() << cache.filePathFromSeed().c_str();

    QByteArray fileContent;
    QFile file(a.applicationFilePath());
    if (file.open(QFile::ReadOnly)) {

        fileContent = file.readAll();
        file.close();
    }

    cache.saveFile(fileContent);
    qDebug() << cache.loadFile().size() << cache.fileHash().c_str();

    return a.exec();
}
