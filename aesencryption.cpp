#include "aesencryption.h"

#include <openssl/conf.h>
#include <openssl/evp.h>

#include <memory>

#include <QDebug>

using EVP_CIPHER_CTX_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

QByteArray aes_encryption::crypt(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv)
{
    return aes_encryption(key, iv).encode(rawText);
}

QByteArray aes_encryption::decrypt(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv)
{
    return aes_encryption(key, iv).decode(rawText);
}

aes_encryption::aes_encryption(const QByteArray &key, const QByteArray &iv)
    : _key(key)
    , _iv(iv.isEmpty() ? key : iv)
{ }

QByteArray aes_encryption::encode(const QByteArray &rawText)
{
    unsigned char* aesKey = (unsigned char*)_key.data();
    unsigned char* aesIv = (unsigned char*)_iv.data();

    EVP_CIPHER_CTX_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_cbc(), 0, aesKey, aesIv);
    if (rc != 1)
        return {};

    QByteArray out;
    out.resize(rawText.size()+16);
    int out_len1 = out.size();

    unsigned char* output = (unsigned char*)out.data();
    unsigned char* input = (unsigned char*)rawText.data();

    rc = EVP_EncryptUpdate(ctx.get(), output, &out_len1, input, rawText.size());
    if (rc != 1)
        return {};

    int out_len2 = out.size() - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), output + out_len1, &out_len2);
    if (rc != 1)
        return {};

    out.resize(out_len1 + out_len2);
    return out;
}

QByteArray aes_encryption::decode(const QByteArray &cipherText)
{
    unsigned char* aesKey = (unsigned char*)_key.data();
    unsigned char* aesIv = (unsigned char*)_iv.data();

    EVP_CIPHER_CTX_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_cbc(), 0, aesKey, aesIv);
    if (rc != 1)
        return {};

    QByteArray out;
    out.resize(cipherText.size());
    int out_len1 = out.size();

    unsigned char* output = (unsigned char*)out.data();
    unsigned char* input = (unsigned char*)cipherText.data();

    rc = EVP_DecryptUpdate(ctx.get(), output, &out_len1, input, cipherText.size());
    if (rc != 1)
        return {};

    int out_len2 = out.size() - out_len1;
    rc = EVP_DecryptFinal_ex(ctx.get(), output + out_len1, &out_len2);
    if (rc != 1)
        return {};

    out.resize(out_len1 + out_len2);
    return out;
}

QByteArray aes_encryption::encode(const QString &rawText)
{
    return encode(rawText.toUtf8());
}

QByteArray aes_encryption::decode(const QString &cipherText)
{
    return decode(cipherText.toUtf8());
}
