#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlField>
#include <QTableView>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegExp>
#include <QDate>
#include <QTime>


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
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();

    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();
    void on_radioButton_7_clicked();
    void on_radioButton_8_clicked();
    void on_radioButton_9_clicked();
    void on_radioButton_10_clicked();
    void on_radioButton_11_clicked();
    void on_radioButton_12_clicked();
    void on_radioButton_13_clicked();
    void on_radioButton_14_clicked();
    void on_radioButton_15_clicked();
    void on_radioButton_16_clicked();
    void on_radioButton_17_clicked();
    void on_radioButton_18_clicked();
    void on_radioButton_19_clicked();
    void on_radioButton_20_clicked();

    void usunRekord(QSqlTableModel* model_tabeli, QTableView* widok_tabeli);
    void dodajPrzedmiot();
    void dodajPracownika();
    void dodajGrupe();
    void dodajZjazd();
    void dodajPrzydzial();
    void usunPrzedmiot();
    void usunPracownika();
    void usunGrupe();
    void usunZjazd();
    void usunPrzydzial();

    void resizeEvent(QResizeEvent* event);

    void programy_menu( const QPoint& );
    void pracownicy_menu( const QPoint& );
    void grupy_menu( const QPoint& );
    void zjazdy_menu( const QPoint& );
    void przydzialy_menu( const QPoint& );
    void on_tabWidget_currentChanged(int index);
    void dodaj_liczbe_godzin(int row, QSqlRecord &record);
    void dodaj_godziny_do_przydzialow();


    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);

    void on_spinBox_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_4_valueChanged(int arg1);
    void on_spinBox_3_valueChanged(int arg1);
    void on_spinBox_5_valueChanged(int arg1);
    void on_spinBox_6_valueChanged(int arg1);
    void on_spinBox_7_valueChanged(int arg1);
    void on_spinBox_8_valueChanged(int arg1);
    void on_spinBox_9_valueChanged(int arg1);
    void on_spinBox_10_valueChanged(int arg1);

    void on_calendarWidget_selectionChanged();

    void on_tableView_4_clicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_checkBox_8_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    QSqlTableModel *program_studiow, *pracownicy, *grupy, *zjazdy, *przydzialy1;

    QSqlRelationalTableModel *przydzialy;

    bool czy_polaczono_z_baza;
    int zakladka;
    QString filtr_rok_studiow;
    QString filtr_typ_zajec;
    QString filtr_stopien;
    QString filtr_zatrudnienie;
    QString filtr_grupa_rok_studiow;
    QString filtr_grupa_typ_zajec;

    bool czy_rok_aktywny[5];
    int gr_cw[5];
    int gr_lab[5];
    int gr_sem[5];
    bool gen_przydzialy[5];

    bool polacz_z_baza();
    void rozlacz_z_baza();

    void odczytaj_przedmioty();
    void odczytaj_pracownikow();
    void odczytaj_grupy();
    void odczytaj_zjazdy();
    void odczytaj_przydzialy();
    void ustaw_tabele();
    void ustaw_szerokosc_kolumn();
    void ustaw_filtr();
    QVector<QStringList> wczytaj_dane_z_pliku(QString sciezka);
    void zapisz_dane_do_pliku(QString sciezka);
    void zapisz_przydzialy(QString sciezka);
    void zapisz_plan(QString sciezka);
    void wstaw_przedmioty_do_bazy(QVector<QStringList> dane);
    void wstaw_pracownikow_do_bazy(QVector<QStringList> dane);
    void wstaw_grupy_do_bazy(QVector<QStringList> dane);
    void wstaw_zjazdy_do_bazy(QVector<QStringList> dane);
    void zakryj_opcje_grup();
    void resetuj_grupy();
    void odczytaj_liczbe_grup();
    void generuj_przydzialy(int rok);

};

#endif // MAINWINDOW_H
