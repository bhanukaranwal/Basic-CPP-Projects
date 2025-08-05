#include "MainWindow.h"
#include "PasswordGenerator.h"
#include "AuthDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QClipboard>
#include <QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(PasswordDatabase *db, QString masterPassword, QWidget *parent)
: QMainWindow(parent), db(db), masterPassword(masterPassword) {
    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Search box
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Search...");
    layout->addWidget(searchEdit);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::search);

    table = new QTableWidget(0, 2);
    table->setHorizontalHeaderLabels({"Site", "Username"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(table);

    // Buttons
    QPushButton *addBtn = new QPushButton("Add");
    QPushButton *removeBtn = new QPushButton("Remove");
    QPushButton *copyBtn = new QPushButton("Copy Password");
    QPushButton *backupBtn = new QPushButton("Backup");
    QPushButton *restoreBtn = new QPushButton("Restore");
    QPushButton *generateBtn = new QPushButton("Generate Password");
    layout->addWidget(addBtn);
    layout->addWidget(removeBtn);
    layout->addWidget(copyBtn);
    layout->addWidget(backupBtn);
    layout->addWidget(restoreBtn);
    layout->addWidget(generateBtn);

    setCentralWidget(central);
    loadEntries();

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addEntry);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeEntry);
    connect(copyBtn, &QPushButton::clicked, this, &MainWindow::copyPassword);
    connect(backupBtn, &QPushButton::clicked, this, &MainWindow::backupDatabase);
    connect(restoreBtn, &QPushButton::clicked, this, &MainWindow::restoreDatabase);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generatePassword);

    // Inactivity timer for auto-lock
    inactivityTimer = new QTimer(this);
    inactivityTimer->setInterval(5 * 60 * 1000); // 5 minutes
    connect(inactivityTimer, &QTimer::timeout, this, &MainWindow::lockApp);
    inactivityTimer->start();
    qApp->installEventFilter(this);
}

void MainWindow::loadEntries(const QString &filter) {
    table->setRowCount(0);
    for (const auto &e : db->entries) {
        if (!filter.isEmpty() && !e.site.contains(filter, Qt::CaseInsensitive)
            && !e.username.contains(filter, Qt::CaseInsensitive))
            continue;
        int r = table->rowCount();
        table->insertRow(r);
        table->setItem(r, 0, new QTableWidgetItem(e.site));
        table->setItem(r, 1, new QTableWidgetItem(e.username));
    }
}

void MainWindow::addEntry() {
    bool ok1, ok2, ok3;
    QString site = QInputDialog::getText(this, "Site", "Site:", QLineEdit::Normal, "", &ok1);
    QString user = QInputDialog::getText(this, "Username", "Username:", QLineEdit::Normal, "", &ok2);
    QString pass = QInputDialog::getText(this, "Password", "Password:", QLineEdit::Normal, "", &ok3);
    if (ok1 && ok2 && ok3 && !site.isEmpty() && !user.isEmpty() && !pass.isEmpty()) {
        db->addEntry({site, user, pass});
        loadEntries(searchEdit->text());
    }
}

void MainWindow::removeEntry() {
    int row = table->currentRow();
    if (row >= 0) {
        db->removeEntry(row);
        loadEntries(searchEdit->text());
    }
}

void MainWindow::copyPassword() {
    int row = table->currentRow();
    if (row >= 0) {
        QString pass = db->entries[row].password;
        QApplication::clipboard()->setText(pass);
        QTimer::singleShot(30000, []() { QApplication::clipboard()->clear(); }); // 30s clear
    }
}

void MainWindow::search(const QString &text) { loadEntries(text); }

void MainWindow::backupDatabase() {
    QString file = QFileDialog::getSaveFileName(this, "Backup Database");
    if (!file.isEmpty())
        db->save(file, masterPassword);
}

void MainWindow::restoreDatabase() {
    QString file = QFileDialog::getOpenFileName(this, "Restore Database");
    if (!file.isEmpty()) {
        db->load(file, masterPassword);
        loadEntries(searchEdit->text());
    }
}

void MainWindow::generatePassword() {
    bool ok;
    int length = QInputDialog::getInt(this, "Password Length", "Length:", 16, 6, 64, 1, &ok);
    if (!ok) return;
    QString pwd = PasswordGenerator::generate(length);
    QInputDialog dlg;
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setLabelText("Generated Password:");
    dlg.setTextValue(pwd);
    dlg.exec();
    // Copy or let user paste as needed
}

bool MainWindow::eventFilter(QObject*, QEvent* event) {
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::KeyPress) {
        resetInactivityTimer();
    }
    return false;
}
void MainWindow::resetInactivityTimer() { inactivityTimer->start(); }
void MainWindow::lockApp() {
    this->hide();
    AuthDialog auth;
    if (auth.exec() == QDialog::Accepted) {
        masterPassword = auth.getMasterPassword();
        db->load("passwords.db", masterPassword);
        this->show();
        resetInactivityTimer();
    } else {
        qApp->quit();
    }
}
