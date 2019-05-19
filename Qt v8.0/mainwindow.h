#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>


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

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void ustaw_przyciski(int id, bool stan);

    void usun_komorke(const QPoint&);

    void add_to_table(int, int);

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
    QVector<QTableWidget*> tabelki;


    bool polacz_z_baza_danych();

    void ustaw_parametry_tabeli();
    void ustaw_parametry_pola_przydzialow();
    void utworz_przycisk_przydzialu(int id, QString etykieta);

    void ustaw_daty_zjazdow();

};

#endif // MAINWINDOW_H


