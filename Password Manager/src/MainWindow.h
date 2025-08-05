#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QTimer>
#include "PasswordDatabase.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(PasswordDatabase *db, QString masterPassword, QWidget *parent = nullptr);
private slots:
    void addEntry();
    void removeEntry();
    void copyPassword();
    void search(const QString &text);
    void backupDatabase();
    void restoreDatabase();
    void generatePassword();
    void resetInactivityTimer();
    void lockApp();

protected:
    bool eventFilter(QObject*, QEvent*) override;

private:
    PasswordDatabase *db;
    QTableWidget *table;
    QLineEdit *searchEdit;
    QTimer *inactivityTimer;
    QString masterPassword;
    void loadEntries(const QString &filter = "");
};
