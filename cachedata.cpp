#include "cachedata.h"
#include "aesencryption.h"

#include <QFile>
#include <QCryptographicHash>

#include <Windows.h>

CacheData::CacheData(const std::string &seed)
    : _seed(seed)
{
    Q_ASSERT_X(!seed.empty(), "CacheData", "seed cannot be empty");
    initFilePath();
    initAesKey();
}

void CacheData::initFilePath()
{
    char tempDirectory[MAX_PATH];
    GetTempPathA(sizeof(tempDirectory), tempDirectory);
    _filePath = tempDirectory;
    _filePath += "\\" + fileNameFromSeed();
}

void CacheData::initAesKey()
{
    const QByteArray& hash = QCryptographicHash::hash(QByteArray::fromStdString(_seed), QCryptographicHash::Sha1).toHex();
    _aesKey = hash.left(16);
    _aesIv = hash.right(16);
}

QByteArray CacheData::randomContent() const
{
    std::string string;
    for (int i = 0; i < 50; i++)
        string += _seed;
    return QByteArray::fromStdString(string);
}

void CacheData::saveFile(const char *filePath, const QByteArray &fileContent)
{
    QFile file(filePath);
    if (file.open(QFile::WriteOnly)) {

        file.write(aes_encryption::crypt(QByteArray(fileContent).append(randomContent()), _aesKey, _aesIv));
        file.close();
    }
}

void CacheData::saveFile(const QByteArray &fileContent)
{
    if (_filePath.empty())
        return;

    saveFile(_filePath.c_str(), fileContent);
}

QByteArray CacheData::loadFile(const char *filePath)
{
    QByteArray fileContent;

    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {

        const auto& content = randomContent();
        fileContent = aes_encryption::decrypt(file.readAll(), _aesKey, _aesIv);
        fileContent = fileContent.remove(fileContent.size() - content.size(), content.size());
        file.close();
    }
    return fileContent;
}

QByteArray CacheData::loadFile()
{
    if (_filePath.empty())
        return {};

    return loadFile(_filePath.c_str());
}

void CacheData::removeFile(const char *filePath)
{
    QFile::setPermissions(filePath, QFile::ReadOther | QFile::WriteOther);
    QFile::remove(filePath);
}

void CacheData::removeFile()
{
    if (_filePath.empty())
        return;

    removeFile(_filePath.c_str());
}

std::string CacheData::fileHash(const char *filePath)
{
    return fileHash(loadFile(filePath));
}

std::string CacheData::fileHash(const QByteArray &fileContent)
{
    if (fileContent.isEmpty())
        return {};

    return QCryptographicHash::hash(fileContent, QCryptographicHash::Md5).toHex();
}

std::string CacheData::fileHash()
{
    return fileHash(_filePath.c_str());
}

std::string CacheData::fileNameFromSeed() const
{
    if (_seed.empty())
        return {};

    const auto& hash = QCryptographicHash::hash(QByteArray::fromStdString(_seed), QCryptographicHash::Sha1).toHex().toUpper();
    return hash.left(qMin(static_cast<int>(_seed.length()), static_cast<int>(hash.size() / 2))).toStdString();
}

std::string CacheData::filePathFromSeed() const
{
    return _filePath;
}

