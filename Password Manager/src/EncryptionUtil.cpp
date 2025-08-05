#include "EncryptionUtil.h"
#include <openssl/evp.h>

static QByteArray evp_crypt(const QByteArray &input, const QString &key, bool encrypt) {
    QByteArray out(input.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int outlen = 0, tmplen = 0;
    unsigned char iv[16] = {0}; // For demo. Use random IV in production.
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.toUtf8().data()), iv, encrypt);
    EVP_CipherUpdate(ctx, reinterpret_cast<unsigned char*>(out.data()), &outlen, reinterpret_cast<const unsigned char*>(input.data()), input.size());
    EVP_CipherFinal_ex(ctx, reinterpret_cast<unsigned char*>(out.data()) + outlen, &tmplen);
    out.resize(outlen + tmplen);
    EVP_CIPHER_CTX_free(ctx);
    return out;
}

QByteArray EncryptionUtil::encrypt(const QByteArray &data, const QString &key) { return evp_crypt(data, key, 1); }
QByteArray EncryptionUtil::decrypt(const QByteArray &data, const QString &key) { return evp_crypt(data, key, 0); }
