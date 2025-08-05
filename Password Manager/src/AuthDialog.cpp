#include "AuthDialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>

AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Master Password:"));
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString AuthDialog::getMasterPassword() const {
    return passwordEdit->text();
}
