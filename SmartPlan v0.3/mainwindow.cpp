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

    QMainWindow::showMaximized();
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
    //ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //ui->tabWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->tabWidget->setMinimumSize(100,100);
    //ui->tabWidget->setMovable(true);
    QList<int> list;
    list.append(MainWindow::width()/7);
    list.append((MainWindow::height()/7)*7);
    ui->splitter->setSizes(list);
    this->setWindowTitle("Smart plan");


    ///automatyczne polaczenie z baza dla testow
#ifdef PL_POLACZAUTOMATYCZNIE
    on_actionConnect_to_database_triggered();
#endif
}

// Destruktor
MainWindow::~MainWindow()
{
    // zamykamy połączenie z bazą
    db.close();
    delete ui;
}


void MainWindow::on_actionConnect_to_database_triggered()
{
    ///zabezpieczenie przed ponownym polaczeniem
    if(db.isOpen())
    {
        QMessageBox::information(this, "Połączenie z bazą", "Połączenie z bazą jest aktywne");
        return;
    }

    if(!polacz_z_baza_danych())
        return;


    QSqlQuery zapytanie,zapytanie_2,zapytanie_3;

    zapytanie.exec("SELECT id_pracownika,id_przedmiotu, id, grupa, do_zaplanowania FROM przydzial_zajec");
    int licznik=0;
    // numerację przydziałów zaczniemy od 1,
    // więc na miejscu 0 wstawiamy dowolny element np. -1
    id_przydzialu.append(-1);
    id_grupy_dla_przydzialu.append(-1);
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
        int grupa = zapytanie.value(3).toInt();
        int do_zaplanowania = zapytanie.value(4).toInt();
        // tworzymy nowy przycisk dla danego przydziału
        utworz_przycisk_przydzialu(id, etykieta, do_zaplanowania);

        // pobieramy identyfikator danego przydziału

        // i dodajemy go do naszej tablicy na miejscu o numerze ,,licznik''
        id_przydzialu.append(id);
        id_grupy_dla_przydzialu.append(grupa);

        qDebug()<<id_przydzialu;
        qDebug()<<id_grupy_dla_przydzialu;
        //wczytaj_plan_zajec(licznik);
        licznik++;
    }

    // Łączymy całą grupę przycisków z funkcją ,,ustaw_przyciski''
    // sygnał ,,buttonToggled'' przekaże do funkcji numer przełączonego przycisku
    // oraz jego stan po przełączeniu (włączony/wyłaczony)
    connect(przyciski_przydzialow, SIGNAL(buttonToggled(int, bool)),this, SLOT(ustaw_przyciski(int,bool)));

    liczba_przydzialow = licznik - 1;


    //qDebug()<<przyciski_przydzialow->button(1)->palette().background().color();
    utworz_tabelki();

    MainWindow::on_tabWidget_tabBarClicked(0);
}


void MainWindow::ustaw_parametry_pola_przydzialow()
{
    //ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setMinimumSize(100,100);

    ui->scrollAreaWidgetContents->setLayout(layout);
    ui->scrollArea->setWidgetResizable(true);
}



void MainWindow::utworz_przycisk_przydzialu(int id, QString etykieta, int do_zaplanowania)
{
    QPushButton * button = new QPushButton(etykieta);

    // tworzymy przycisk i dodajemy go do grupy
    przyciski_przydzialow->addButton(button, id);

    /// ustawiamy rozmiar na minimal
    przyciski_przydzialow->button(id)->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    // zmieniamy zwykły przycisk na ,,przełącznik''
    przyciski_przydzialow->button(id)->setCheckable(true);


    QLabel * label = new QLabel;
    label->setParent(przyciski_przydzialow->button(id));
    label->setText(QString::number(do_zaplanowania));
    label->setStyleSheet("font-size:20px");


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
        for(auto przycisk : przyciski_przydzialow->buttons())
        {
            int i = przyciski_przydzialow->id(przycisk);
            if(i != id) przyciski_przydzialow->button(i)->setChecked(false);
        }
        // kontrolnie wyświetlamy id przydziału
        // odpowiadającego klikniętemu przyciskowi
        // później można to zakomentować !!!
        //qDebug()<<"Przydzial "<<id_przydzialu[id];
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
        //qDebug()<<"Connected!";
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
    sprawdz_polaczenie();


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

    QSqlQuery zapytanie, zapytanie1;

    zapytanie.exec("SELECT id_przydzialu FROM plan_zajec WHERE dzien="+ QString::number(id_dnia[item2->column()]) +" AND od_godzina="+ QString::number(item2->row()+godzina_poczatkowa) +" AND id_grupy="+ QString::number(id_grupy[ui->tabWidget->currentIndex()]));
    zapytanie.first();

    QList<QLabel*> etykieta = przyciski_przydzialow->button(zapytanie.value(0).toInt())->findChildren<QLabel*>();

    QLabel * label = etykieta.last();
    label->setText( QString::number(label->text().toInt()+1));

    zapytanie1.exec("UPDATE przydzial_zajec SET do_zaplanowania = do_zaplanowania + 1 WHERE id="+ QString::number(zapytanie.value(0).toInt()));
    zapytanie1.exec("UPDATE plan_zajec SET id_przydzialu=NULL, sala=NULL WHERE dzien="+ QString::number(id_dnia[item2->column()]) +" AND od_godzina="+ QString::number(item2->row()+godzina_poczatkowa) +" AND id_grupy="+ QString::number(id_grupy[ui->tabWidget->currentIndex()]));

    table->setItem(item2->row(), item2->column(), nullptr);
}

void MainWindow::add_to_table(int row, int column)
{

    if(sprawdz_polaczenie()) return;

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
            QList<QLabel*> etykieta = button->findChildren<QLabel*>();
            QLabel * label = etykieta.last();
            if(label->text().toInt() <= 0)
            {
                button->setChecked(false);
                //QColor color =  button->palette().background().color().dark();
                //qDebug()<<color.name();
                //button->setStyleSheet("background-color: "+color.name());
                return;
            }

            else
            {
                QColor color = button->palette().background().color();
                QString text = button->text();
                item->setBackgroundColor(color);
                item->setText(text);
                QSqlQuery zapytanie;
                zapytanie.exec("UPDATE plan_zajec SET id_przydzialu="+ QString::number(przyciski_przydzialow->checkedId()) +" WHERE dzien="+ QString::number(id_dnia[item->column()]) +" AND od_godzina="+ QString::number(item->row()+godzina_poczatkowa) +" AND id_grupy="+ QString::number(id_grupy[ui->tabWidget->currentIndex()]));
                QList<QLabel*> etykieta = button->findChildren<QLabel*>();
                QLabel * label = etykieta.last();
                label->setText( QString::number(label->text().toInt()-1));
                zapytanie.exec("UPDATE przydzial_zajec SET do_zaplanowania = do_zaplanowania - 1 WHERE id="+ QString::number(przyciski_przydzialow->checkedId()));
            }
        }
    }
    else QMessageBox::warning(this, "Database error", "Database disconnected, check your internet connection");
}

void MainWindow::wczytaj_plan_zajec(int licznik)
{

    QSqlQuery zapytanie;
    QString query;
    query+="select dzien, id_przydzialu, od_godzina, sala from plan_zajec";
    query+=" join przydzial_zajec on przydzial_zajec.id = id_przydzialu";
    query+=" where grupa = %1 AND id_przydzialu NOTNULL";
    query=query.arg(QString::number(id_grupy[licznik]));
    //qDebug()<<query;


    //zapytanie.exec("select dzien, id_przydzialu, od_godzina, sala from plan_zajec where id_przydzialu IN (SELECT id FROM przydzial_zajec WHERE grupa = " + QString::number(id_grupy[licznik]) + ")" );
    //qDebug()<<"select dzien, id_przydzialu, od_godzina, sala from plan_zajec where id_przydzialu IN (SELECT id FROM przydzial_zajec WHERE grupa = " + QString::number(id_grupy[licznik])+ ")";
    zapytanie.exec(query);
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


        zapytanie.exec("SELECT data, id FROM zjazdy");

        QStringList daty;
        while(zapytanie.next())
        {
            ile_zjazdow++;
            daty.append(zapytanie.value(0).toString());
            id_dnia.push_back(zapytanie.value(1).toInt());
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
    if(sprawdz_polaczenie()) clearDatabase();
}



void MainWindow::clearDatabase()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Czyszczenie planu lekcji", "Czy napewno wyczyścić plan lekcji? \n Wszystkie aktualne dane zostaną utracone.", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QSqlQuery clear, ilosc_grup_zapytanie, ilosc_zjazdow_zapytanie, utworz_rekord, ilosc_przydzialow, zaktualizuj_liczbe_godzin, sortuj, ile;


        /////////////sortuj.exec("SELECT * FROM zjazdy ORDER BY data"); co z tym?
        /////////////sortuj.next();

        ilosc_przydzialow.exec("SELECT COUNT(*) FROM przydzial_zajec");
        ilosc_przydzialow.next();
        ilosc_grup_zapytanie.exec("SELECT COUNT(*) FROM grupy");
        ilosc_grup_zapytanie.next();
        //qDebug()<<"Ilosc grup: "<<ilosc_grup.value(0).toInt();
        ilosc_zjazdow_zapytanie.exec("SELECT COUNT(*) FROM zjazdy");
        ilosc_zjazdow_zapytanie.next();
        //qDebug()<<"Ilosc zjazdow: "<<ilosc_zjazdow.value(0).toInt();
        ile.exec("SELECT COUNT(*) FROM plan_zajec WHERE id_przydzialu NOTNULL");
        ile.next();

        QString ile_wartosc = ile.value(0).toString();
        int ilosc_grup = ilosc_grup_zapytanie.value(0).toInt();
        int ilosc_zjazdow = ilosc_zjazdow_zapytanie.value(0).toInt();
        int ilosc_przydzialow_wartosc = ilosc_przydzialow.value(0).toInt();
        float progressBarValue = ilosc_grup * ilosc_zjazdow * (godzina_koncowa-godzina_poczatkowa);
        float licznik=0;
        float valueToUpdate=0;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Czyszczenie planu lekcji", "Zostanie usunięte "+ ile_wartosc +" komórek. \n Kontynuować?" , QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {


            QWidget * widget = new QWidget;
            QVBoxLayout * layout = new QVBoxLayout;
            QProgressBar * progressBar = new QProgressBar;
            QLabel * label = new QLabel;
            widget->setLayout(layout);
            layout->addWidget(label);
            layout->addWidget(progressBar);
            progressBar->setMaximum(1000);
            widget->resize(400,100);
            label->setText("Czyszczenie planu lekcji, proszę czekać");
            widget->setWindowTitle("Czyszcenie planu lekcji");
            widget->show();



            //qDebug()<<ilosc_przydzialow.value(0).toInt();
            //qDebug()<<ilosc_przydzialow_wartosc;


            clear.exec("DELETE FROM plan_zajec");

            for(int i=0; i<ilosc_przydzialow_wartosc; i++)
            {
                zaktualizuj_liczbe_godzin.exec("UPDATE przydzial_zajec SET do_zaplanowania = (SELECT liczba_godzin FROM programy_studiow WHERE id =( SELECT id_przedmiotu FROM przydzial_zajec WHERE id="+ QString::number(id_przydzialu[i]) +")) WHERE id="+QString::number(id_przydzialu[i]));
                //qDebug()<<"UPDATE przydzial_zajec SET do_zaplanowania = (SELECT liczba_godzin FROM programy_studiow WHERE id =( SELECT id_przedmiotu FROM przydzial_zajec WHERE id="+ QString::number(id_przydzialu[i]) +")) WHERE id="+QString::number(id_przydzialu[i]);
            }

            for(int i=0; i<ilosc_grup; i++)
            {
                for(int j=0; j<=ilosc_zjazdow; j++)
                {
                    for(int k=0; k<godzina_koncowa-godzina_poczatkowa; k++)
                    {
                        qApp->processEvents();
                        utworz_rekord.exec("INSERT INTO plan_zajec (dzien, id_przydzialu, od_godzina, do_godzina, sala, id_grupy) VALUES(" + QString::number(j) + ", NULL, " + QString::number(k+godzina_poczatkowa) + ", " + QString::number(k+1+godzina_poczatkowa) + ", NULL," + QString::number(id_grupy[i]) +")");
                        //progressBar->setValue(progressBarValue);
                        licznik++;
                        valueToUpdate = (licznik/progressBarValue)*1000;
                        progressBar->setValue(static_cast<int>(valueToUpdate));
                        progressBar->update();

                    }
                }
            }

            licznik=0;
            widget->hide();
            qApp->quit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

        }
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{

    if(przyciski_przydzialow->checkedButton())
    {
        QAbstractButton * button = przyciski_przydzialow->checkedButton();
        button->setChecked(false);
    }
    qDebug()<<"Changed";
    for (int i = 1; i < id_przydzialu.size(); i++)
    {
        qDebug()<<id_grupy[index];
        qDebug()<<id_grupy_dla_przydzialu[i];
        qDebug()<<" ";
        if(id_grupy[index] == id_grupy_dla_przydzialu[i]) przyciski_przydzialow->button(id_przydzialu[i])->show();
        else przyciski_przydzialow->button(id_przydzialu[i])->hide();
    }
}

bool MainWindow::sprawdz_polaczenie()
{
    if(db.open()) return true;

    else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Błąd połączenia", "Połączenie z bazą zostało zerwane. \n Połączyć ponownie?" , QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            polacz_z_baza_danych();
        }
        return false;
    }
}


void MainWindow::on_actionRefresh_database_triggered()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_actionClose_triggered()
{
    qApp->quit();
}
