#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include "qsqlerror.h"
#include <ctime>
#include <cstdlib>
#define PL_POLACZAUTOMATYCZNIE

// Konstruktor naszej głównej klasy
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);


    // liczba elementow tabeli Przydzialy_zajec
    liczba_przydzialow = 0;

    // parametry do etykiet wierszy tabeli
    godzina_poczatkowa = 8;
    godzina_koncowa = 20;
    godzina_skok = 1;

    // tworzymy layout dla przycisków przydziału zajęć
    layout=new QVBoxLayout();

    // tworzymy grupę przycisków
    przyciski_przydzialow = new QButtonGroup();

    // wyłączamy ekskluzywność
    // (bo inaczej jeden przycisk musi być zawsze włączony),
    // teraz można włączyć więcej niż jeden przycisk,
    // ale my i tak do tego nie dopuścimy
    przyciski_przydzialow->setExclusive(false);

    // ustawiamy parametry związane z tabelą
    // oraz polem przycisków przydziałów zajęć
    ustaw_parametry_pola_przydzialow();
    ///wylaczenie zaznaczenia

    ///wylaczenie scrollbara
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    ui->tabWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->tabWidget->setMinimumSize(100,100);
    ui->tabWidget->setMovable(true);


    ///automatyczne polaczenie z baza dla testow
#ifdef PL_POLACZAUTOMATYCZNIE
    on_pushButton_3_clicked();
#endif
}

// Destruktor
MainWindow::~MainWindow()
{
    // zamykamy połączenie z bazą
    db.close();
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    ///zabezpieczenie przed ponownym polaczeniem
    if(!db.isOpen())
    {
        if(!polacz_z_baza_danych()){
            return;
        }

        QSqlQuery zapytanie,zapytanie_2,zapytanie_3;

        zapytanie.exec("SELECT id_pracownika,id_przedmiotu, id FROM przydzial_zajec");
        int licznik=1;
        // numerację przydziałów zaczniemy od 1,
        // więc na miejscu 0 wstawiamy dowolny element np. -1
        id_przydzialu.append(-1);
        while(zapytanie.next())
        {
            QString pracownik=zapytanie.value(0).toString();
            QString s="SELECT imie,nazwisko FROM pracownicy WHERE id = " + pracownik;
            QString przedmiot=zapytanie.value(1).toString();
            QString z="SELECT nazwa_przedmiotu FROM programy_studiow WHERE id = " + przedmiot;
            zapytanie_2.exec(s);
            zapytanie_2.next();
            zapytanie_3.exec(z);
            zapytanie_3.next();
            QString etykieta = QString(zapytanie_2.value(0).toString() + "\n" + zapytanie_2.value(1).toString()+"\n"+zapytanie_3.value(0).toString());
            // tworzymy nowy przycisk dla danego przydziału
            utworz_przycisk_przydzialu(licznik, etykieta);
            // pobieramy identyfikator danego przydziału
            int id = zapytanie.value(2).toInt();
            // i dodajemy go do naszej tablicy na miejscu o numerze ,,licznik''
            id_przydzialu.append(id);
            licznik++;
        }

        // Łączymy całą grupę przycisków z funkcją ,,ustaw_przyciski''
        // sygnał ,,buttonToggled'' przekaże do funkcji numer przełączonego przycisku
        // oraz jego stan po przełączeniu (włączony/wyłaczony)
        connect(przyciski_przydzialow, SIGNAL(buttonToggled(int, bool)),this, SLOT(ustaw_przyciski(int,bool)));

        liczba_przydzialow = licznik - 1;
    }

    else {
        QMessageBox::information(this, "Database connected", "Juz polaczono z baza");
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    db.close();
    qDebug() << "Closed";
}




void MainWindow::ustaw_parametry_pola_przydzialow()
{
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setMinimumSize(100,100);

    ui->scrollAreaWidgetContents->setLayout(layout);
    ui->scrollArea->setWidgetResizable(true);
}


// Funkcja tworząca nowy przycisk przydziału
// i dodająca go do grupy przycisków
// id - identyfikator przycisku w grupie
// etykieta - napis na przycisku
void MainWindow::utworz_przycisk_przydzialu(int id, QString etykieta)
{
    // tworzymy przycisk i dodajemy go do grupy
    przyciski_przydzialow->addButton(new QPushButton(etykieta), id);

    /// ustawiamy rozmiar na minimal
    przyciski_przydzialow->button(id)->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    // zmieniamy zwykły przycisk na ,,przełącznik''
    przyciski_przydzialow->button(id)->setCheckable(true);

    //dodajemy przycisk do layoutu
    layout->addWidget(przyciski_przydzialow->button(id));

    //ustawiamy kolor przycisku
    QColor color;
        if (id % 1 == 0) color.setRgb(255,100,rand()%100+155);
        if (id % 2 == 0) color.setRgb(255,rand()%100+155,100);
        if (id % 3 == 0) color.setRgb(100,255,rand()%100+155);
        if (id % 4 == 0) color.setRgb(100,rand()%100+155,255);
        if (id % 5 == 0) color.setRgb(rand()%100+155,100,255);
        if (id % 6 == 0) color.setRgb(rand()%100+155,255,100);
    QString qss = QString("background-color: %1").arg(color.name());
    przyciski_przydzialow->button(id)->setStyleSheet(qss);
}


// Funkcja wyłącza wszystkie przyciski oprócz klikniętego
// id - identyfikator klikniętego przycisku
// stan - stan przycisku po kliknięciu (włączony/wyłączony)
void MainWindow::ustaw_przyciski(int id, bool stan)
{
    //jeżeli użytkownik włączył przycisk o numerze ,,id''
    if(stan)
    {
        // to wyłączamy wszystkie pozostałe
        for(int i=1; i<=liczba_przydzialow; i++)
            if(i != id) przyciski_przydzialow->button(i)->setChecked(false);

        // kontrolnie wyświetlamy id przydziału
        // odpowiadającego klikniętemu przyciskowi
        // później można to zakomentować !!!
        qDebug()<<"Przydzial "<<id_przydzialu[id];
    }
    // jeżeli uzytkownik wyłączył przycisk,
    // to z pozostałymi nic nie trzeba robić


}














bool MainWindow::polacz_z_baza_danych()
{

    db.setHostName("serwer1866993.home.pl");
    db.setDatabaseName("28568274_postgres");
    db.setUserName("28568274_postgres");
    db.setPassword("!QAZXSW@2wsxzaq1");

    if (db.open())
    {
        qDebug() << "Opened connection!";

        //taby

        QSqlQuery zapytanie;

        zapytanie.exec("SELECT rok_studiow, nazwa FROM grupy");
        int licznik=0;
        // numerację przydziałów zaczniemy od 1,
        // więc na miejscu 0 wstawiamy dowolny element np. -1

        while(zapytanie.next())
        {
            QString rok_studiow=zapytanie.value(0).toString();
            QString grupa_nazwa=zapytanie.value(1).toString();
            QWidget *widget= new QWidget;
            ui->tabWidget->addTab(widget,"rok "+rok_studiow+" grupa "+grupa_nazwa);
            QVBoxLayout *layout = new QVBoxLayout;
            widget->setLayout(layout);

            QTableWidget *table = new QTableWidget;
            tabelki.push_back(table);
            layout->addWidget(table);
            table->setRowCount(10);
            table->setColumnCount(10);

            table->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
            table->setMinimumSize(100,100);



            QStringList godziny;
            for(int i=godzina_poczatkowa; i<godzina_koncowa; i+=godzina_skok)
            {
                godziny.append(QString::number(i) + "-" + QString::number(i+godzina_skok));
            }
            table->setRowCount(godziny.length());
            table->setVerticalHeaderLabels(godziny);

            QSqlQuery zapytanie;
            int ile_zjazdow = 0;


            zapytanie.exec("SELECT data FROM zjazdy");

            QStringList daty;
            while(zapytanie.next())
            {
                ile_zjazdow++;
                daty.append(zapytanie.value(0).toString());
            }
            table->setColumnCount(ile_zjazdow);
            table->setHorizontalHeaderLabels(daty);







            table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            table->setSelectionMode(QAbstractItemView::NoSelection);
            connect(table, SIGNAL(cellClicked(int ,int )), this, SLOT(add_to_table(int , int )));
            table->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(table, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(usun_komorke(const QPoint &)));
            licznik++;
        }

        return true;
    }
    else
    {
        QMessageBox::information(this, "Blad polaczenia", "Sprawdz polaczenie z internetem");
        return false;
    }



}

void MainWindow::usun_komorke(const QPoint & pos)
{

    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if( table == nullptr )
    {
        return;
    }
    QTableWidgetItem * item2 = table->itemAt(pos);
    if( item2 == nullptr )
    {
        return;
    }

    table->setItem(item2->row(), item2->column(), nullptr);

    ///qDebug()<<"tak";
}

void MainWindow::add_to_table(int row, int column)
{

    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if( table == nullptr )
    {
        return;
    }

    QTableWidgetItem * item =  table->item(row,column);
    if(!item)
    {
        item = new QTableWidgetItem();
        table->setItem(row, column, item);
    }

    ///ustawienie button na aktualnie zaznaczony i zamiana kolaru i nazwy komorki, przy wybranym przycisku

    if(przyciski_przydzialow->checkedButton())
    {
        QAbstractButton * button = przyciski_przydzialow->checkedButton();
        item->setBackgroundColor(button->palette().background().color());
        item->setText(button->text());
    }
}

