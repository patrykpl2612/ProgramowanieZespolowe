#include "mainwindow.h"
#include "ui_mainwindow.h"

static QLabel *createDragLabel(const QString &text, QWidget *parent)
{
    QLabel *label = new QLabel(text, parent);
    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::Panel);
    label->setFrameShadow(QFrame::Raised);
    return label;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    QLabel* etykieta = createDragLabel("ghgjvjjh",this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
  /*  if (event->mimeData()->hasFormat("text/uri-list"))
            event->acceptProposedAction();

            */
    if (event->mimeData()->hasText()) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
  /*  QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;

        QString fileName = urls.first().toLocalFile();
        if (fileName.isEmpty())
            return;
            */

       // if (readFile(fileName))
           // setWindowTitle(tr("%1 - %2").arg(fileName)
             //                           .arg(tr("Drag File")));
           // ui->label->setText(fileName);
          //  ui->label->setPixmap(fileName);

            if (event->mimeData()->hasText()) {
                const QMimeData *mime = event->mimeData();
               // QStringList pieces = mime->text().split(QRegularExpression(QStringLiteral("\\s+")),
                 //                    QString::SkipEmptyParts);
                QPoint position = event->pos();
                QPoint hotSpot;

                QByteArrayList hotSpotPos = mime->data("application/x-hotspot").split(' ');
                if (hotSpotPos.size() == 2) {
                    hotSpot.setX(hotSpotPos.first().toInt());
                    hotSpot.setY(hotSpotPos.last().toInt());
                }


              //  for (const QString &piece : pieces) {
                    QLabel *newLabel = createDragLabel(mime->text(),this);
                    newLabel->move(position);
                    newLabel->show();
                    newLabel->setAttribute(Qt::WA_DeleteOnClose);

                    position += QPoint(newLabel->width(), 0);
                //}

                if (event->source() == this) {
                    event->setDropAction(Qt::MoveAction);
                    event->accept();
                } else {
                    event->acceptProposedAction();
                }
            } else {
                event->ignore();
            }
            for (QWidget *widget : findChildren<QWidget *>()) {
                if (!widget->isVisible())
                    widget->deleteLater();
            }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = qobject_cast<QLabel*>(childAt(event->pos()));
    if (!child)
      {  qDebug()<<"Nie";
        return;
    }
    //else
        qDebug()<<"Tak";
        //return;

    QPoint hotSpot = event->pos() - child->pos();

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(child->text());
    mimeData->setData("application/x-hotspot",
                      QByteArray::number(hotSpot.x()) + ' ' + QByteArray::number(hotSpot.y()));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction)
        child->close();

}


