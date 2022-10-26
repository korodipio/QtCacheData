#ifndef QAESENCRYPTION_H
#define QAESENCRYPTION_H

#include <QByteArray>
#include <QVector>
#include <QVarLengthArray>

class aes_encryption
{
public:
    static QByteArray crypt(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv = {});
    static QByteArray decrypt(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv = {});

    aes_encryption(const QByteArray& key, const QByteArray& iv = {});
    ~aes_encryption() = default;

    QByteArray encode(const QByteArray &rawText);
    QByteArray decode(const QByteArray &cipherText);

    QByteArray encode(const QString &rawText);
    QByteArray decode(const QString &cipherText);

private:
    QByteArray _key;
    QByteArray _iv;
};


#endif // QAESENCRYPTION_H
