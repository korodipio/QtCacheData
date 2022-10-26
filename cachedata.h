#ifndef CACHEDATA_H
#define CACHEDATA_H

#include <string>

#include <QByteArray>

class CacheData
{
public:
    CacheData(const std::string& seed);

    void saveFile(const char* filePath, const QByteArray& fileContent);
    void saveFile(const QByteArray& fileContent);

    QByteArray loadFile(const char* filePath);
    QByteArray loadFile();

    void removeFile(const char* filePath);
    void removeFile();

    std::string fileHash(const char* filePath);
    std::string fileHash(const QByteArray& fileContent);
    std::string fileHash();

    std::string fileNameFromSeed() const;
    std::string filePathFromSeed() const;

protected:
    void initFilePath();
    void initAesKey();
    QByteArray randomContent() const;

protected:
    std::string _seed;
    std::string _filePath;
    QByteArray _aesKey;
    QByteArray _aesIv;

private:
    Q_DISABLE_COPY_MOVE(CacheData)
    CacheData() = default;
};

#endif // CACHEDATA_H
