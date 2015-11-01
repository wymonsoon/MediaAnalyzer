#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "mediafile.h"
#include <QList>
#include <QUrl>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle("Media Analyzer");
    // ### Debug cmd
    QString fileName("J:\\Blues\\Asian\\Censored\\CMN-117\\CMN-117.mp4");
    setWindowTitle(tr("%1 - %2").arg(fileName)
                                .arg(tr("Drag File")));
    readFile(fileName);
    // ###
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list"))
           event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
       return;
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;
    if (readFile(fileName)) {
        setWindowTitle(tr("%1 - %2").arg(fileName)
                                    .arg(tr("Drag File")));
    }
}

bool MainWindow::readFile(const QString &fileName) {
    MediaFile media(fileName);
    return true;
}

bool MainWindow::analyzeFile(const QString &fileName){
    return true;
}
