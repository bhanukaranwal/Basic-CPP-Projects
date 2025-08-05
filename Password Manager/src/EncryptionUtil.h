#pragma once
#include <QString>
#include <QByteArray>

class EncryptionUtil {
public:
    static QByteArray encrypt(const QByteArray &data, const QString &key);
    static QByteArray decrypt(const QByteArray &encrypted, const QString &key);
};
