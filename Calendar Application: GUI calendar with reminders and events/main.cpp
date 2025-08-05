// main.cpp
#include <QApplication>
#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QInputDialog>
#include <QMap>
#include <QDate>
#include <QMessageBox>

class CalendarApp : public QWidget {
    Q_OBJECT
public:
    CalendarApp(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Calendar with Reminders");
        resize(420, 480);

        QVBoxLayout* layout = new QVBoxLayout(this);
        calendar = new QCalendarWidget(this);
        eventsBox = new QTextEdit(this);
        eventsBox->setReadOnly(true);

        QPushButton* addBtn = new QPushButton("Add Event", this);
        QPushButton* delBtn = new QPushButton("Delete Event", this);

        layout->addWidget(calendar);
        layout->addWidget(eventsBox);
        layout->addWidget(addBtn);
        layout->addWidget(delBtn);

        connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarApp::showEvents);
        connect(addBtn, &QPushButton::clicked, this, &CalendarApp::addEvent);
        connect(delBtn, &QPushButton::clicked, this, &CalendarApp::deleteEvent);

        showEvents();
    }

private slots:
    void showEvents() {
        QDate sel = calendar->selectedDate();
        QStringList list = events[sel];
        eventsBox->clear();
        if (list.isEmpty()) {
            eventsBox->setText("No events yet.");
        } else {
            eventsBox->setText(list.join("\n"));
        }
    }
    void addEvent() {
        QDate sel = calendar->selectedDate();
        bool ok;
        QString text = QInputDialog::getText(this, "Add Event",
                    "Event/Reminder:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            events[sel] << text;
            showEvents();
        }
    }
    void deleteEvent() {
        QDate sel = calendar->selectedDate();
        if (events[sel].isEmpty()) return;
        bool ok;
        QStringList list = events[sel];
        QString item = QInputDialog::getItem(this, "Delete Event",
                "Select event to delete:", list, 0, false, &ok);
        if (ok) {
            events[sel].removeAll(item);
            if (events[sel].isEmpty())
                events.remove(sel);
            showEvents();
        }
    }
private:
    QCalendarWidget* calendar;
    QTextEdit* eventsBox;
    QMap<QDate, QStringList> events;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalendarApp w;
    w.show();
    return a.exec();
}
