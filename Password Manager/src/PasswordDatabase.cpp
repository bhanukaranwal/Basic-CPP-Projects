#include "PasswordDatabase.h"
#include "EncryptionUtil.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

bool PasswordDatabase::load(const QString &filename, const QString &masterPassword) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QByteArray encrypted = file.readAll();
    QByteArray decrypted = EncryptionUtil::decrypt(encrypted, masterPassword);
    QJsonDocument doc = QJsonDocument::fromJson(decrypted);
    if (!doc.isArray()) return false;
    entries.clear();
    QJsonArray arr = doc.array();
    for (const auto &v : arr) {
        QJsonObject obj = v.toObject();
        entries.append({obj["site"].toString(), obj["username"].toString(), obj["password"].toString()});
    }
    return true;
}

bool PasswordDatabase::save(const QString &filename, const QString &masterPassword) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QJsonArray arr;
    for (const auto &e : entries) {
        QJsonObject obj;
        obj["site"] = e.site;
        obj["username"] = e.username;
        obj["password"] = e.password;
        arr.append(obj);
    }
    QJsonDocument doc(arr);
    QByteArray encrypted = EncryptionUtil::encrypt(doc.toJson(), masterPassword);
    file.write(encrypted);
    return true;
}

void PasswordDatabase::addEntry(const PasswordEntry &entry) { entries.append(entry); }
void PasswordDatabase::removeEntry(int index) { entries.remove(index); }
