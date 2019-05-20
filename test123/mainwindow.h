#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegExp>


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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void resizeEvent(QResizeEvent* event);
    void on_radioButton_2_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();
    void on_radioButton_7_clicked();
    void on_radioButton_8_clicked();
    void on_radioButton_9_clicked();
    void programy_menu( const QPoint& );
    void on_radioButton_10_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void usunRekord();
    void dodajRekord();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    QSqlTableModel *program_studiow;

    bool czy_polaczono_z_baza;
    QString filtr_rok_studiow;
    QString filtr_typ_zajec;

    bool polacz_z_baza();
    void rozlacz_z_baza();

    void odczytaj_przedmioty();
    void ustaw_tabele();
    void ustaw_szerokosc_kolumn();
    void ustaw_filtr();
    QVector<QStringList> wczytaj_dane_z_pliku(QString sciezka);
    void wstaw_przedmioty_do_bazy(QVector<QStringList> dane);
};

#endif // MAINWINDOW_H
