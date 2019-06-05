#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    if(db.isOpen())
        return;

    if(!polacz_z_baza_danych())
        return;


    QSqlQuery zapytanie,zapytanie_2,zapytanie_3;

    zapytanie.exec("SELECT id_pracownika,id_przedmiotu, id FROM przydzial_zajec");
    int licznik=0;
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
        int id = zapytanie.value(2).toInt();
        // tworzymy nowy przycisk dla danego przydziału
        utworz_przycisk_przydzialu(id, etykieta);

        // pobieramy identyfikator danego przydziału

        // i dodajemy go do naszej tablicy na miejscu o numerze ,,licznik''
        id_przydzialu.append(id);
        qDebug()<<id;
        //wczytaj_plan_zajec(licznik);
        licznik++;
    }

    // Łączymy całą grupę przycisków z funkcją ,,ustaw_przyciski''
    // sygnał ,,buttonToggled'' przekaże do funkcji numer przełączonego przycisku
    // oraz jego stan po przełączeniu (włączony/wyłaczony)
    connect(przyciski_przydzialow, SIGNAL(buttonToggled(int, bool)),this, SLOT(ustaw_przyciski(int,bool)));

    liczba_przydzialow = licznik - 1;


    qDebug()<<przyciski_przydzialow->button(1)->palette().background().color();
    utworz_tabelki();


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



void MainWindow::utworz_przycisk_przydzialu(int id, QString etykieta)
{
    QPushButton * button = new QPushButton(etykieta);

    // tworzymy przycisk i dodajemy go do grupy
    przyciski_przydzialow->addButton(button, id);

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
    QString qss = QString("background-color: %1; margin-bottom: 5px;").arg(color.name());

    przyciski_przydzialow->button(id)->setStyleSheet(qss);
    przyciski_przydzialow->button(id)->show();

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
        for(int i=1; i<=liczba_przydzialow+1; i++)
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
}

void MainWindow::add_to_table(int row, int column)
{

    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if( table == nullptr )
    {
        return;
    }

    if(db.open())
    {
        QTableWidgetItem * item =  table->item(row,column);
        if(!item)
        {
            item = new QTableWidgetItem();
            table->setItem(row, column, item);
        }

        if(przyciski_przydzialow->checkedButton())
        {
            QAbstractButton * button = przyciski_przydzialow->checkedButton();
            QColor color = button->palette().background().color();
            QString text = button->text();
            item->setBackgroundColor(color);
            item->setText(text);

            QSqlQuery zapytanie;
            zapytanie.exec("UPDATE plan_zajec SET id_przydzialu="+ QString::number(przyciski_przydzialow->checkedId()) +" WHERE dzien="+ QString::number(item->column()) +" AND od_godzina="+ QString::number(item->row()+godzina_poczatkowa) +" AND id_grupy="+ QString::number(ui->tabWidget->currentIndex()));


        }
    }
    else QMessageBox::warning(this, "Database error", "Database disconnected, check your internet connection");
}

void MainWindow::wczytaj_plan_zajec(int licznik)
{

    QSqlQuery zapytanie;

    zapytanie.exec("select dzien, id_przydzialu, od_godzina, sala from plan_zajec where id_przydzialu IN (SELECT id FROM przydzial_zajec WHERE grupa = " + QString::number(id_grupy[licznik]) + ")" );


    while(zapytanie.next())
    {

        int column=zapytanie.value(0).toInt();
        int id_przydzialu_tmp = zapytanie.value(1).toInt();
        int row=zapytanie.value(2).toInt();

        QTableWidgetItem *item= new QTableWidgetItem;
        tabelki[licznik]->setItem(row-godzina_poczatkowa, column-1, item);


        QAbstractButton * button = przyciski_przydzialow->button(id_przydzialu_tmp);
        //qDebug()<<id_przydzialu_tmp;
        //qDebug()<<button->palette().background().color();
        //qDebug()<<item;
        item->setBackgroundColor(button->palette().background().color());
        item->setText(button->text());


    }
}

int MainWindow::utworz_tabelki(){


    QSqlQuery zapytanie;

    zapytanie.exec("SELECT rok_studiow, nazwa, id FROM grupy");
    int licznik=0;


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
        id_grupy.push_back(zapytanie.value(2).toInt());
        layout->addWidget(table);
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

        wczytaj_plan_zajec(licznik);
        licznik++;
    }
    return 0;
}

void MainWindow::on_actionClear_database_triggered()
{
    clearDatabase();
}



void MainWindow::clearDatabase()
{
    QMessageBox::warning(this, "Warning", "Do you really want to clear database?");
    QSqlQuery clear, ilosc_grup_zapytanie, ilosc_zjazdow_zapytanie, utworz_rekord;
    clear.exec("DELETE FROM plan_zajec");
    ilosc_grup_zapytanie.exec("SELECT COUNT(*) FROM grupy");
    ilosc_grup_zapytanie.next();
    //qDebug()<<"Ilosc grup: "<<ilosc_grup.value(0).toInt();
    ilosc_zjazdow_zapytanie.exec("SELECT COUNT(*) FROM zjazdy");
    ilosc_zjazdow_zapytanie.next();
    //qDebug()<<"Ilosc zjazdow: "<<ilosc_zjazdow.value(0).toInt();
    QWidget * widget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;
    QProgressBar * progressBar = new QProgressBar;
    widget->setLayout(layout);
    layout->addWidget(progressBar);
    //widget->show();

    int ilosc_grup = ilosc_grup_zapytanie.value(0).toInt();
    int ilosc_zjazdow = ilosc_zjazdow_zapytanie.value(0).toInt();

    int progressBarValue = ilosc_grup * ilosc_zjazdow * (godzina_koncowa-godzina_poczatkowa);

    for(int i=0; i<ilosc_grup; i++)
    {
        for(int j=0; j<ilosc_zjazdow; j++)
        {
            for(int k=0; k<godzina_koncowa-godzina_poczatkowa; k++)
            {
                utworz_rekord.exec("INSERT INTO plan_zajec (dzien, id_przydzialu, od_godzina, do_godzina, sala, id_grupy) VALUES(" + QString::number(j) + ", NULL, " + QString::number(k+godzina_poczatkowa) + ", " + QString::number(k+1+godzina_poczatkowa) + ", NULL," + QString::number(id_grupy[i]) +")");
                //progressBar->setValue(progressBarValue);
            }
        }
    }

}
