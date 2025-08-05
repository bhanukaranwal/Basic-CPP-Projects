// main.cpp
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QDate>

class TaskScheduler : public QWidget {
    Q_OBJECT
public:
    TaskScheduler(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Task Scheduler");

        QVBoxLayout* layout = new QVBoxLayout(this);

        // Task entry area
        QHBoxLayout* entry = new QHBoxLayout();
        descEdit = new QLineEdit(this);
        dateEdit = new QDateEdit(QDate::currentDate(), this);
        dateEdit->setCalendarPopup(true);
        QPushButton* addBtn = new QPushButton("Add Task", this);
        entry->addWidget(new QLabel("Task:"));
        entry->addWidget(descEdit);
        entry->addWidget(new QLabel("Due Date:"));
        entry->addWidget(dateEdit);
        entry->addWidget(addBtn);

        taskList = new QListWidget(this);

        // Edit/delete controls
        QHBoxLayout* ctrl = new QHBoxLayout();
        QPushButton* editBtn = new QPushButton("Edit Task", this);
        QPushButton* delBtn = new QPushButton("Delete Task", this);
        ctrl->addWidget(editBtn);
        ctrl->addWidget(delBtn);

        layout->addLayout(entry);
        layout->addWidget(taskList);
        layout->addLayout(ctrl);

        connect(addBtn, &QPushButton::clicked, this, &TaskScheduler::addTask);
        connect(editBtn, &QPushButton::clicked, this, &TaskScheduler::editTask);
        connect(delBtn, &QPushButton::clicked, this, &TaskScheduler::deleteTask);
    }

private slots:
    void addTask() {
        QString desc = descEdit->text();
        QDate date = dateEdit->date();
        if (desc.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a task description.");
            return;
        }
        QString item = desc + " | Due: " + date.toString("yyyy-MM-dd");
        taskList->addItem(item);
        descEdit->clear();
    }
    void editTask() {
        QListWidgetItem* item = taskList->currentItem();
        if (!item) return;
        bool ok;
        QString text = QInputDialog::getText(this, "Edit Task", "Task:",
                                             QLineEdit::Normal, item->text(), &ok);
        if (ok && !text.isEmpty()) {
            item->setText(text);
        }
    }
    void deleteTask() {
        delete taskList->currentItem();
    }
private:
    QLineEdit* descEdit;
    QDateEdit* dateEdit;
    QListWidget* taskList;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TaskScheduler w;
    w.show();
    return a.exec();
}
