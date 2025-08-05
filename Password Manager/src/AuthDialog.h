#pragma once
#include <QDialog>
class QLineEdit;
class AuthDialog : public QDialog {
    Q_OBJECT
public:
    explicit AuthDialog(QWidget *parent = nullptr);
    QString getMasterPassword() const;
private:
    QLineEdit *passwordEdit;
};
