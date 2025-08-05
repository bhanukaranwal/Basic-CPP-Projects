// main.cpp
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPixmap>

class DrawingApp : public QWidget {
    Q_OBJECT

public:
    DrawingApp(QWidget* parent = nullptr) : QWidget(parent),
        drawing(false), brushColor(Qt::black), brushSize(4) {
        setWindowTitle("Simple Drawing Application");

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        QHBoxLayout* controls = new QHBoxLayout();

        colorBtn = new QPushButton("Pick Color", this);
        clearBtn = new QPushButton("Clear", this);
        sizeSlider = new QSlider(Qt::Horizontal, this);
        sizeSlider->setRange(1, 20);
        sizeSlider->setValue(brushSize);
        sizeLbl = new QLabel(QString("Brush Size: %1").arg(brushSize), this);

        controls->addWidget(colorBtn);
        controls->addWidget(clearBtn);
        controls->addWidget(sizeLbl);
        controls->addWidget(sizeSlider);

        mainLayout->addLayout(controls);

        canvas = QPixmap(800, 500);
        canvas.fill(Qt::white);
        lastPoint = QPoint(-1, -1);
        setFixedSize(820, 550);

        connect(colorBtn, &QPushButton::clicked, this, [=]() {
            QColor picked = QColorDialog::getColor(brushColor, this, "Select Brush Color");
            if (picked.isValid())
                brushColor = picked;
        });
        connect(clearBtn, &QPushButton::clicked, this, [=]() {
            canvas.fill(Qt::white);
            update();
        });
        connect(sizeSlider, &QSlider::valueChanged, this, [=](int v) {
            brushSize = v;
            sizeLbl->setText(QString("Brush Size: %1").arg(v));
        });
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawPixmap(10, 40, canvas);
    }
    void mousePressEvent(QMouseEvent* e) override {
        if (e->button() == Qt::LeftButton && inCanvas(e->pos())) {
            drawing = true;
            lastPoint = toCanvas(e->pos());
        }
    }
    void mouseMoveEvent(QMouseEvent* e) override {
        if (drawing && inCanvas(e->pos())) {
            QPoint pt = toCanvas(e->pos());
            QPainter p(&canvas);
            p.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(lastPoint, pt);
            lastPoint = pt;
            update();
        }
    }
    void mouseReleaseEvent(QMouseEvent* e) override {
        if (drawing && e->button() == Qt::LeftButton) {
            drawing = false;
        }
    }

private:
    QPushButton* colorBtn;
    QPushButton* clearBtn;
    QSlider* sizeSlider;
    QLabel* sizeLbl;
    QPixmap canvas;
    QColor brushColor;
    int brushSize;
    bool drawing;
    QPoint lastPoint;

    bool inCanvas(const QPoint& p) const {
        return (p.x() >= 10 && p.x() < 810 && p.y() >= 40 && p.y() < 540);
    }
    QPoint toCanvas(const QPoint& p) const {
        return QPoint(p.x() - 10, p.y() - 40);
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DrawingApp w;
    w.show();
    return a.exec();
}
