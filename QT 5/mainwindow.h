#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton * przyciski[90];

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void ustaw_przyciski();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

};

#endif // MAINWINDOW_H
