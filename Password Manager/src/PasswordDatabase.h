#pragma once
#include <QString>
#include <QVector>
struct PasswordEntry {
    QString site, username, password;
};

class PasswordDatabase {
public:
    bool load(const QString &filename, const QString &masterPassword);
    bool save(const QString &filename, const QString &masterPassword) const;
    void addEntry(const PasswordEntry &entry);
    void removeEntry(int index);
    QVector<PasswordEntry> entries;
};
