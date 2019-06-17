#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>
#include <ctime>
#include <cstdlib>
#include <QApplication>
#include <QProcess>
#include <QLabel>
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include "qprogressbar.h"
#include "qtimer.h"
#include "qthread.h"
#include "qtooltip.h"
#include <QtPrintSupport/QPrinter>
#include "qtextdocument.h"
#include "qfile.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void ustaw_przyciski(int , bool);

    void usun_komorke(const QPoint&);

    void add_to_table(int, int);void on_actionClear_database_triggered();

    void wczytaj_plan_zajec(int);

    void on_tabWidget_tabBarClicked(int index);


    void on_actionConnect_to_database_triggered();

    void on_actionRefresh_database_triggered();

    void on_actionClose_triggered();

    void on_actionExport_to_PDF_triggered();

private:
    Ui::MainWindow *ui;

    int liczba_przydzialow;
    int godzina_poczatkowa;
    int godzina_koncowa;
    int godzina_skok;

    QVBoxLayout *layout;
    QButtonGroup *przyciski_przydzialow;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    // Tablica przechowująca identyfikatory przydziałów zajęć
    // dla każdego utworzonego przycisku,
    // tzn. id_przydzialu[i] zawiera id przydziału połączonego z i-tym przyciskiem
    QVector<int> id_przydzialu;
    QVector<int> id_grupy_dla_przydzialu;
    QVector<int> id_grupy;
    QVector<int> id_dnia;
    QVector<QTableWidget*> tabelki;


    bool polacz_z_baza_danych();

    int utworz_tabelki();
    void ustaw_parametry_tabeli();
    void ustaw_parametry_pola_przydzialow();
    void utworz_przycisk_przydzialu(int id, QString etykieta, int do_zaplanowania);
    void clearDatabase();
    void ustaw_daty_zjazdow();
    bool sprawdz_polaczenie();

};

#endif // MAINWINDOW_H


