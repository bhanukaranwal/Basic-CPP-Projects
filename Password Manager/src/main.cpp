#include <QApplication>
#include "AuthDialog.h"
#include "MainWindow.h"
#include "PasswordDatabase.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString dbFile = "passwords.db";
    PasswordDatabase db;

    AuthDialog auth;
    if (auth.exec() == QDialog::Accepted) {
        QString masterPass = auth.getMasterPassword();
        db.load(dbFile, masterPass);
        MainWindow w(&db, masterPass);
        w.show();
        int ret = app.exec();
        db.save(dbFile, masterPass);
        return ret;
    }
    return 0;
}
