// main.cpp

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QChar>
#include <QDebug>

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Basic Calculator");
        setFixedSize(300, 400);

        display = new QLineEdit(this);
        display->setReadOnly(true);
        display->setAlignment(Qt::AlignRight);
        display->setFixedHeight(35);
        display->setStyleSheet("font-size: 18px;");

        QGridLayout* layout = new QGridLayout(this);
        layout->addWidget(display, 0, 0, 1, 4);

        // Button texts arranged in order
        const QString buttons[4][4] = {
            {"7", "8", "9", "/"},
            {"4", "5", "6", "*"},
            {"1", "2", "3", "-"},
            {"C", "0", "=", "+"}
        };

        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                QPushButton* btn = new QPushButton(buttons[r][c], this);
                btn->setFixedSize(60, 60);
                btn->setStyleSheet("font-size: 16px;");
                layout->addWidget(btn, r + 1, c);
                connect(btn, &QPushButton::clicked, this, &Calculator::buttonClicked);
            }
        }
    }

private slots:
    void buttonClicked() {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        if (!btn) return;

        QString val = btn->text();

        if (val == "C") {
            currentExpression.clear();
            display->setText("");
        } else if (val == "=") {
            // Evaluate expression
            double result;
            bool ok = evaluateExpression(currentExpression, result);
            if (ok)
                display->setText(QString::number(result));
            else
                display->setText("Error");
            currentExpression.clear();
        } else {
            currentExpression += val;
            display->setText(currentExpression);
        }
    }

private:
    QLineEdit* display;
    QString currentExpression;

    // Simple evaluator: supports + - * /, no parentheses, left-to-right evaluation
    bool evaluateExpression(const QString& expr, double& result) {
        // Tokenize expression
        QVector<QString> tokens;
        QString num;
        for (int i = 0; i < expr.length(); ++i) {
            QChar ch = expr[i];
            if (ch.isDigit() || ch == '.') {
                num += ch;
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                if (num.isEmpty()) return false;
                tokens.append(num);
                tokens.append(QString(ch));
                num.clear();
            } else {
                return false; // invalid character
            }
        }
        if (!num.isEmpty())
            tokens.append(num);

        if (tokens.isEmpty()) return false;

        // Evaluate left to right:
        bool ok;
        result = tokens[0].toDouble(&ok);
        if (!ok) return false;

        for (int i = 1; i < tokens.size(); i += 2) {
            QString op = tokens[i];
            double operand = tokens[i+1].toDouble(&ok);
            if (!ok) return false;
            if (op == "+") result += operand;
            else if (op == "-") result -= operand;
            else if (op == "*") result *= operand;
            else if (op == "/") {
                if (operand == 0) return false;
                result /= operand;
            } else return false;
        }
        return true;
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator calculator;
    calculator.show();
    return a.exec();
}
