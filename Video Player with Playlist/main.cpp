// main.cpp
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QMediaPlaylist>

class VideoPlayer : public QWidget {
    Q_OBJECT
public:
    VideoPlayer(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Video Player with Playlist");
        resize(800, 600);

        QVBoxLayout* layout = new QVBoxLayout(this);

        // Video screen
        videoWidget = new QVideoWidget(this);
        player = new QMediaPlayer(this);
        playlist = new QMediaPlaylist(this);
        player->setPlaylist(playlist);
        player->setVideoOutput(videoWidget);

        // Playlist UI
        listWidget = new QListWidget(this);

        // Controls
        QHBoxLayout* ctl = new QHBoxLayout();
        QPushButton* addBtn = new QPushButton("Add Videos");
        QPushButton* playBtn = new QPushButton("Play");
        QPushButton* pauseBtn = new QPushButton("Pause");
        QPushButton* nextBtn = new QPushButton("Next");
        QPushButton* prevBtn = new QPushButton("Previous");

        ctl->addWidget(addBtn);
        ctl->addWidget(playBtn);
        ctl->addWidget(pauseBtn);
        ctl->addWidget(prevBtn);
        ctl->addWidget(nextBtn);

        layout->addWidget(videoWidget);
        layout->addLayout(ctl);
        layout->addWidget(listWidget);

        connect(addBtn, &QPushButton::clicked, this, &VideoPlayer::addFiles);
        connect(playBtn, &QPushButton::clicked, player, &QMediaPlayer::play);
        connect(pauseBtn, &QPushButton::clicked, player, &QMediaPlayer::pause);
        connect(nextBtn, &QPushButton::clicked, playlist, &QMediaPlaylist::next);
        connect(prevBtn, &QPushButton::clicked, playlist, &QMediaPlaylist::previous);

        connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &VideoPlayer::playlistChanged);
    }

private slots:
    void addFiles() {
        QStringList files = QFileDialog::getOpenFileNames(this, "Select Video Files", "",
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
            "Video Files (*.mp4 *.avi *.mkv *.wmv *.mov);;All Files (*)"
#else
            "All Files (*.*)"
#endif
        );
        for (const QString &file : files) {
            playlist->addMedia(QUrl::fromLocalFile(file));
            listWidget->addItem(file);
        }
        if (!files.isEmpty() && player->state() == QMediaPlayer::StoppedState) {
            playlist->setCurrentIndex(0);
            player->play();
        }
    }
    void playlistChanged(int idx) {
        listWidget->setCurrentRow(idx);
    }
private:
    QVideoWidget* videoWidget;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    QListWidget* listWidget;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    VideoPlayer w;
    w.show();
    return a.exec();
}
