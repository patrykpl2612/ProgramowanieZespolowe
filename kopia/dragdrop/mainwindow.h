#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDrag>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QFile>
#include<QMouseEvent>
#include<QDebug>
#include<QLabel>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
