#include "mainwindow.h"
#include "comboboxitemdelegate.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    czy_polaczono_z_baza = false;
    filtr_rok_studiow = ""; //wszystkie
    filtr_typ_zajec = ""; //wszystkie
    filtr_stopien= ""; //wszystkie
    filtr_zatrudnienie=""; //wszystkie
    filtr_grupa_rok_studiow="";//wszystkie
    filtr_grupa_typ_zajec="";//wszystkie

   ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(programy_menu( const QPoint& )));
   ui->tableView_2->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView_2, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(pracownicy_menu( const QPoint& )));
   ui->tableView_5->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView_5, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(grupy_menu( const QPoint& )));
   ui->tableView_3->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView_3, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(zjazdy_menu( const QPoint& )));
   ui->tableView_4->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView_4, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(przydzialy_menu( const QPoint& )));



   ComboBoxItemDelegate* combo_stopien = new ComboBoxItemDelegate(0, ui->tableView_2);
   ui->tableView_2->setItemDelegateForColumn(3, combo_stopien);
   ComboBoxItemDelegate* combo_stanowisko = new ComboBoxItemDelegate(1, ui->tableView_2);
   ui->tableView_2->setItemDelegateForColumn(4, combo_stanowisko);

   on_tabWidget_currentChanged(ui->tabWidget->currentIndex());

   //ui->calendarWidget->setEnabled(false);

   for(int i=0; i<5; i++)
       czy_rok_aktywny[i] = true;
   odczytaj_liczbe_grup();
   gen_przydzialy[1] = ui->checkBox_5->isChecked();
   gen_przydzialy[2] = ui->checkBox_6->isChecked();
   gen_przydzialy[3] = ui->checkBox_7->isChecked();
   gen_przydzialy[4] = ui->checkBox_8->isChecked();

   this->setWindowTitle("SmartAssignments");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   ustaw_szerokosc_kolumn();
}


//Połącz z bazą
void MainWindow::on_pushButton_clicked()
{
    polacz_z_baza();
    if(czy_polaczono_z_baza)
    {
        ui->label->setText("Połączono");
        ui->label->setStyleSheet("QLabel { color : green; }");
        odczytaj_przedmioty();
        odczytaj_pracownikow();
        odczytaj_grupy();
        odczytaj_zjazdy();
        odczytaj_przydzialy();
        ustaw_szerokosc_kolumn();
    }
    else
    {
        ui->label->setText("Błąd połączenia");
        ui->label->setStyleSheet("QLabel { color : red; }");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    rozlacz_z_baza();
    ui->label->setText("Rozłączono");
    ui->label->setStyleSheet("QLabel { color : red; }");
}

bool MainWindow::polacz_z_baza()
{

    /*
    db.setHostName("localhost");
    db.setDatabaseName("plan_NSI");
    db.setUserName("postgres");
    db.setPassword("ketrab");

*/
    db.setHostName("serwer1866993.home.pl");
    db.setDatabaseName("28568274_postgres");
    db.setUserName("28568274_postgres");
    db.setPassword("!QAZXSW@2wsxzaq1");

    if (db.open())
    {
        qDebug() << "Opened connection!";
        czy_polaczono_z_baza = true;
        return true;
    }
    else
    {
        qDebug() << "Something went wrong :c";
        qDebug()<< db.lastError().databaseText()<<endl;
        return false;
    }
}

void MainWindow::rozlacz_z_baza()
{
    if(czy_polaczono_z_baza)
    {
        db.close();
        czy_polaczono_z_baza = false;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    qApp->quit();
}

void MainWindow::odczytaj_przedmioty()
{
    if(czy_polaczono_z_baza)
    {
        program_studiow = new QSqlTableModel(this,db);
        program_studiow->setTable("programy_studiow");
        program_studiow->setEditStrategy(QSqlTableModel::OnFieldChange);
        program_studiow->setSort(0, Qt::AscendingOrder);
        program_studiow->select();
        program_studiow->setHeaderData(0, Qt::Horizontal, "Id");
        program_studiow->setHeaderData(1, Qt::Horizontal, "Rok studiow");
        program_studiow->setHeaderData(2, Qt::Horizontal, "Nazwa przedmiotu");
        program_studiow->setHeaderData(3, Qt::Horizontal, "Liczba godzin");
        program_studiow->setHeaderData(4, Qt::Horizontal, "Typ zajęć");
        ui->tableView->setModel(program_studiow);
        ui->tableView->show();
       // ustaw_tabele();
    }
}

void MainWindow::odczytaj_pracownikow()
{
    if(czy_polaczono_z_baza)
    {
        pracownicy = new QSqlTableModel(this,db);
        pracownicy->setTable("pracownicy");
        pracownicy->setEditStrategy(QSqlTableModel::OnFieldChange);
        pracownicy->setSort(0, Qt::AscendingOrder);
        pracownicy->select();
        pracownicy->setHeaderData(0, Qt::Horizontal, "Id");
        pracownicy->setHeaderData(1, Qt::Horizontal, "Imię");
        pracownicy->setHeaderData(2, Qt::Horizontal, "Nazwisko");
        pracownicy->setHeaderData(3, Qt::Horizontal, "Stopień");
        pracownicy->setHeaderData(4, Qt::Horizontal, "Stanowisko");
        ui->tableView_2->setModel(pracownicy);
        ui->tableView_2->show();
       // ustaw_tabele();
    }
}

void MainWindow::odczytaj_grupy()
{
    if(czy_polaczono_z_baza)
    {
        grupy = new QSqlTableModel(this,db);
        grupy->setTable("grupy");
        grupy->setEditStrategy(QSqlTableModel::OnFieldChange);
        grupy->setSort(0, Qt::AscendingOrder);
        grupy->select();
        grupy->setHeaderData(0, Qt::Horizontal, "Id");
        grupy->setHeaderData(1, Qt::Horizontal, "Rok studiow");
        grupy->setHeaderData(2, Qt::Horizontal, "Nazwa grupy");
        ui->tableView_5->setModel(grupy);
        ui->tableView_5->show();
       // ustaw_tabele();
    }
}

void MainWindow::odczytaj_zjazdy()
{
    if(czy_polaczono_z_baza)
    {
        zjazdy = new QSqlTableModel(this,db);
        zjazdy->setTable("zjazdy");
        zjazdy->setEditStrategy(QSqlTableModel::OnFieldChange);
        zjazdy->setSort(1, Qt::AscendingOrder);
        zjazdy->select();
        zjazdy->setHeaderData(0, Qt::Horizontal, "Id");
        zjazdy->setHeaderData(1, Qt::Horizontal, "Data");
        ui->tableView_3->setModel(zjazdy);
        ui->tableView_3->show();

       // ustaw_tabele();
    }
}

void MainWindow::odczytaj_przydzialy()
{
    if(czy_polaczono_z_baza)
    {
        przydzialy = new QSqlRelationalTableModel(this,db);
        przydzialy->setTable("przydzial_zajec");
        przydzialy->setJoinMode(QSqlRelationalTableModel::LeftJoin);
        przydzialy->setRelation(1,QSqlRelation("pracownicy","id","nazwisko"));
        przydzialy->setRelation(2,QSqlRelation("programy_studiow","id","nazwa_przedmiotu"));
        przydzialy->setRelation(3,QSqlRelation("grupy","id","nazwa"));
        przydzialy->setEditStrategy(QSqlTableModel::OnFieldChange);
        przydzialy->setSort(0, Qt::AscendingOrder);
        przydzialy->select();
        przydzialy->setHeaderData(0, Qt::Horizontal, "Id");
       // przydzialy->setHeaderData(1, Qt::Horizontal, "Imię");
        przydzialy->setHeaderData(1, Qt::Horizontal, "Prowadzący");
        przydzialy->setHeaderData(2, Qt::Horizontal, "Nazwa przedmiotu");
       // przydzialy->setHeaderData(4, Qt::Horizontal, "Typ zajęć");
        przydzialy->setHeaderData(3, Qt::Horizontal, "Grupa");
        przydzialy->setHeaderData(4, Qt::Horizontal, "Do zaplanowania");
        ui->tableView_4->setModel(przydzialy);

        ui->tableView_4->setColumnHidden(4,true);
        ui->tableView_4->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_4));

        ui->tableView_4->show();

        //connect(przydzialy, SIGNAL(afterUpdate(int row, QSqlRecord &record)), this, SLOT(dodaj_liczbe_godzin(int row, QSqlRecord &record)));
        //connect(ui->tableView_4, SIGNAL(clicked(const QModelIndex &index)), this, SLOT(dodaj_godziny_do_przydzialow()));

        // ustaw_tabele();
    }
}

//dodawanie liczby godzin do zaplanowania do tabeli przydzialy
void MainWindow::dodaj_godziny_do_przydzialow()
{
    QSqlQuery zapytanie,zapytanie1,zapytanie2;
    QString ss = "SELECT id_przedmiotu FROM przydzial_zajec";
    QString s = "SELECT liczba_godzin FROM programy_studiow WHERE id = ";
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(ss);
        while(zapytanie.next())
        {
                QString id = zapytanie.value(0).toString();
                if(id!="0")
                {
                    QString ss2 = s + id;
                   // qDebug()<<ss2;
                    zapytanie1.exec(ss2);
                    zapytanie1.next();
                    //qDebug()<<zapytanie1.value(0);
                    QString godz = zapytanie1.value(0).toString();
                    QString ss3 = "UPDATE przydzial_zajec SET do_zaplanowania = " + godz + " WHERE id_przedmiotu = " + id;
                    zapytanie2.exec(ss3);
                    //qDebug()<<ss3;
                }
        }
    }
}

void MainWindow::generuj_przydzialy(int rok)
{
    QSqlQuery zapytanie,zapytanie1,zapytanie2;
    QString ss = "SELECT id, rok_studiow, liczba_godzin, typ_zajec FROM programy_studiow";
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(ss);
        while(zapytanie.next())
        {
            int rok_studiow = zapytanie.value(1).toInt();
            if(rok_studiow == rok)
            {
                QString id_przedmiotu = zapytanie.value(0).toString();
                QString do_zaplanowania = zapytanie.value(2).toString();
                QString typ_zajec = zapytanie.value(3).toString().simplified(); //simplified usuwa zbedne spacje
                QString grupa;

                if (typ_zajec == "w")
                {
                    //Pobieramy id grupy wykładowej
                    QString ss1 = "SELECT id FROM grupy WHERE rok_studiow = " + QString::number(rok) + " AND nazwa = 'wyk'" ;
                    zapytanie1.exec(ss1);
                    zapytanie1.next();
                    QString id_grupa_wyk = zapytanie1.value(0).toString();
                    QString ss2 = "INSERT INTO przydzial_zajec(id_pracownika, id_przedmiotu, grupa, do_zaplanowania) VALUES(NULL,"+id_przedmiotu+","+id_grupa_wyk+","+do_zaplanowania+")";
                    zapytanie2.exec(ss2);
                }

                if (typ_zajec == "l")
                {
                    qDebug()<<gr_lab[rok];
                    for(int j = 1; j<=gr_lab[rok]; j++)
                    {
                        QString nazwa_grupy = "lab" + QString::number(j);
                        QString ss1 = "SELECT id FROM grupy WHERE rok_studiow = " + QString::number(rok) + " AND nazwa = '"+nazwa_grupy +"'";
                        //qDebug()<<ss1;
                        zapytanie1.exec(ss1);
                        zapytanie1.next();
                        QString id_grupa_lab = zapytanie1.value(0).toString();
                        //qDebug()<<id_grupa_lab;
                        QString ss2 = "INSERT INTO przydzial_zajec(id_pracownika, id_przedmiotu, grupa, do_zaplanowania) VALUES(NULL,"+id_przedmiotu+","+id_grupa_lab+","+do_zaplanowania+")";
                        //qDebug()<<ss2;
                        zapytanie2.exec(ss2);
                    }
                }
                if (typ_zajec == "ć")
                {
                    qDebug()<<gr_cw[rok];
                    for(int j = 1; j<=gr_cw[rok]; j++)
                    {
                        QString nazwa_grupy = "gr" + QString::number(j);
                        QString ss1 = "SELECT id FROM grupy WHERE rok_studiow = " + QString::number(rok) + " AND nazwa = '"+nazwa_grupy +"'";
                        //qDebug()<<ss1;
                        zapytanie1.exec(ss1);
                        zapytanie1.next();
                        QString id_grupa_lab = zapytanie1.value(0).toString();
                        //qDebug()<<id_grupa_lab;
                        QString ss2 = "INSERT INTO przydzial_zajec(id_pracownika, id_przedmiotu, grupa, do_zaplanowania) VALUES(NULL,"+id_przedmiotu+","+id_grupa_lab+","+do_zaplanowania+")";
                        //qDebug()<<ss2;
                        zapytanie2.exec(ss2);
                    }
                }
                if (typ_zajec == "s")
                {
                    //qDebug()<<gr_sem[rok];
                    for(int j = 1; j<=gr_sem[rok]; j++)
                    {
                        QString nazwa_grupy = "sem" + QString::number(j);
                        QString ss1 = "SELECT id FROM grupy WHERE rok_studiow = " + QString::number(rok) + " AND nazwa = '"+nazwa_grupy +"'";
                        //qDebug()<<ss1;
                        zapytanie1.exec(ss1);
                        zapytanie1.next();
                        QString id_grupa_lab = zapytanie1.value(0).toString();
                        //qDebug()<<id_grupa_lab;
                        QString ss2 = "INSERT INTO przydzial_zajec(id_pracownika, id_przedmiotu, grupa, do_zaplanowania) VALUES(NULL,"+id_przedmiotu+","+id_grupa_lab+","+do_zaplanowania+")";
                        //qDebug()<<ss2;
                        zapytanie2.exec(ss2);
                    }
                }


            }             //qDebug()<<ss3;

        }
        przydzialy->select();
    }
}

void MainWindow::ustaw_tabele()
{
    QSortFilterProxyModel *proxy1=new QSortFilterProxyModel();
    proxy1->setSourceModel(program_studiow);
    proxy1->setHeaderData(0, Qt::Horizontal, "Id");
    proxy1->setHeaderData(1, Qt::Horizontal, "Rok studiow");
    proxy1->setHeaderData(2, Qt::Horizontal, "Nazwa przedmiotu");
    proxy1->setHeaderData(3, Qt::Horizontal, "Liczba godzin");
    proxy1->setHeaderData(4, Qt::Horizontal, "Typ zajęć");
    QStandardItemModel *modd=new QStandardItemModel();

    for (int z =0; z< proxy1->rowCount(); ++z)
       {
        for (int y =0; y< proxy1->columnCount(); ++y)
            {
            QStandardItem *item= new QStandardItem();
            item->setText(proxy1->index(z,y).data().toString());
            item->setTextAlignment(Qt::AlignCenter);
            modd->setItem(z,y,item);

            }
      }
    ui->tableView->setModel(modd);
}

void MainWindow::ustaw_szerokosc_kolumn()
{
    // przedmioty
    int szer = ui->tableView->width();

    ui->tableView->setColumnWidth(0, szer/20);
    ui->tableView->setColumnWidth(1, 4*szer/20);
    ui->tableView->setColumnWidth(2, 9*szer/20);
    ui->tableView->setColumnWidth(3, 3*szer/20);
    ui->tableView->setColumnWidth(4, 3*szer/20);

    // pracownicy
    int szer2 = ui->tableView_2->width();

    ui->tableView_2->setColumnWidth(0, szer2/20);
    ui->tableView_2->setColumnWidth(1, 4*szer2/20);
    ui->tableView_2->setColumnWidth(2, 9*szer2/20);
    ui->tableView_2->setColumnWidth(3, 2*szer2/20);
    ui->tableView_2->setColumnWidth(4, 4*szer2/20);

    // grupy
    int szer3 = ui->tableView_5->width();

    ui->tableView_5->setColumnWidth(0, szer3/10);
    ui->tableView_5->setColumnWidth(1, 2*szer3/10);
    ui->tableView_5->setColumnWidth(2, 7*szer3/10);

    // zjazdy
    int szer4 = ui->tableView_3->width();

    ui->tableView_3->setColumnWidth(0, 2*szer4/10);
    ui->tableView_3->setColumnWidth(1, 8*szer4/10);

    // pracownicy
    int szer5 = ui->tableView_4->width();

    ui->tableView_4->setColumnWidth(0, szer5/20);
    ui->tableView_4->setColumnWidth(1, 6*szer5/20);
    ui->tableView_4->setColumnWidth(2, 9*szer5/20);
    ui->tableView_4->setColumnWidth(3, 4*szer5/20);


    //automatycznie
   /* ui->tableView->resizeColumnsToContents();
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_3->resizeColumnsToContents();
    ui->tableView_4->resizeColumnsToContents();
    ui->tableView_5->resizeColumnsToContents();
*/
}

// Menu kontekstowe dla tabeli pracownicy
void MainWindow::pracownicy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń pracownika", this);
    QAction *dodaj = new QAction("Dodaj nowego pracownika", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunPracownika()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajPracownika()));

    if(czy_polaczono_z_baza)
    {
        menu->addAction(dodaj);
        menu->addAction(usun);
        menu->popup(ui->tableView_2->viewport()->mapToGlobal(pos));
    }

}

// Menu kontekstowe dla tabeli programy studiów
void MainWindow::programy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń przedmiot", this);
    QAction *dodaj = new QAction("Dodaj nowy przedmiot", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunPrzedmiot()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajPrzedmiot()));

    if(czy_polaczono_z_baza)
    {
        menu->addAction(dodaj);
        menu->addAction(usun);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }

}

// Menu kontekstowe dla tabeli grupy
void MainWindow::grupy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń grupę", this);
    QAction *dodaj = new QAction("Dodaj nową grupę", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunGrupe()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajGrupe()));

    menu->addAction(dodaj);
    menu->addAction(usun);
    menu->popup(ui->tableView_5->viewport()->mapToGlobal(pos));
}

// Menu kontekstowe dla tabeli zjazdy
void MainWindow::zjazdy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń zjazd", this);
    QAction *dodaj = new QAction("Dodaj nowy zjazd", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunZjazd()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajZjazd()));

    menu->addAction(dodaj);
    menu->addAction(usun);
    menu->popup(ui->tableView_3->viewport()->mapToGlobal(pos));
}

// Menu kontekstowe dla tabeli przydzialy
void MainWindow::przydzialy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń przydział", this);
    QAction *dodaj = new QAction("Dodaj nowy przydział", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunPrzydzial()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajPrzydzial()));

    menu->addAction(dodaj);
    menu->addAction(usun);
    menu->popup(ui->tableView_4->viewport()->mapToGlobal(pos));
}

// Dodaj nowy rekord do tabeli
void MainWindow::on_pushButton_5_clicked()
{
  if(czy_polaczono_z_baza)
  {
    if(zakladka == 0)
        dodajPracownika();
    else if(zakladka == 1)
        dodajPrzedmiot();
    else if(zakladka == 2)
        dodajGrupe();
    else if(zakladka == 3)
        dodajZjazd();
    else if(zakladka == 4)
    {    dodajPrzydzial();
         dodaj_godziny_do_przydzialow();
    }
  }

    /* QSqlRecord rekord = program_studiow->record();
    rekord.setValue(1,1); //rok studiow
    rekord.setValue(2,"lalalala"); //nazwa przedmiotu
    rekord.setValue(3,30); //liczba godzin
    rekord.setValue(4,'w'); //typ zajęć

    if(program_studiow->insertRecord(-1, rekord))
    {
        qDebug()<<"successful insertion";
    //    program_studiow->submit();
    }
    else
    {
        qDebug() << program_studiow->lastError().databaseText()<<endl;
        db.rollback();
    } */
}

// usuń rekord z tabeli
void MainWindow::on_pushButton_6_clicked()
{
  if(czy_polaczono_z_baza)
  {
    if(zakladka == 0)
        usunPracownika();
    else if(zakladka == 1)
        usunPrzedmiot();
    else if(zakladka == 2)
        usunGrupe();
    else if(zakladka == 3)
        usunZjazd();
    else if(zakladka == 4)
        usunPrzydzial();
  }
}


void MainWindow::usunRekord(QSqlTableModel* model_tabeli, QTableView* widok_tabeli)
{
    QModelIndexList zaznaczone_wiersze;

    zaznaczone_wiersze = widok_tabeli->selectionModel()->selectedRows();
    if(zaznaczone_wiersze.count() > 0)
    {
        if(QMessageBox::question(this,"Potwierdzenie","Czy na pewno chcesz usunąć te rekordy?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            for(int i=0; i<zaznaczone_wiersze.count(); i++)
            {
                int zaznaczony_wiersz = zaznaczone_wiersze.at(i).row();
                model_tabeli->removeRow(zaznaczony_wiersz);
            }
            model_tabeli->select();
        }
    }
    else
    {
        QMessageBox::warning(this,"Błąd","Zaznacz wiersz, który chcesz usunąć", QMessageBox::Ok);
    }

}

void MainWindow::usunPrzedmiot()
{
    usunRekord(program_studiow, ui->tableView);
}

void MainWindow::usunPracownika()
{
    usunRekord(pracownicy, ui->tableView_2);
}

void MainWindow::usunGrupe()
{
    usunRekord(grupy, ui->tableView_5);
}

void MainWindow::usunZjazd()
{
    usunRekord(zjazdy, ui->tableView_3);
}

void MainWindow::usunPrzydzial()
{
    usunRekord(przydzialy, ui->tableView_4);
}

void MainWindow::dodajPrzedmiot()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO programy_studiow(rok_studiow,nazwa_przedmiotu, liczba_godzin, typ_zajec) VALUES (NULL,NULL,NULL,NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    program_studiow->select();
}

void MainWindow::dodajPracownika()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO pracownicy(imie, nazwisko, stopien, stanowisko) VALUES (NULL,NULL,NULL,NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    pracownicy->select();
}

void MainWindow::dodajGrupe()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO grupy(rok_studiow,nazwa) VALUES (NULL,NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    grupy->select();
}

void MainWindow::dodajZjazd()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO zjazdy(data) VALUES (NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    zjazdy->select();
}

void MainWindow::dodajPrzydzial()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO przydzial_zajec(id_pracownika, id_przedmiotu, grupa, do_zaplanowania) VALUES (NULL, NULL, NULL, NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    przydzialy->select();
}

void MainWindow::dodaj_liczbe_godzin(int row, QSqlRecord &record)
{
    qDebug()<<"hhhh";
    record.setValue("do_zaplanowania",46);
}

void MainWindow::on_pushButton_7_clicked()
{
    QString sciezka = QFileDialog::getOpenFileName(this,"Otwórz plik z danymi","./","Pliki tekstowe (*.txt *.csv)");
    if(!sciezka.isNull())
    {
        QVector<QStringList> dane = wczytaj_dane_z_pliku(sciezka);
        if(!dane.isEmpty() && czy_polaczono_z_baza)
        {
            if(zakladka == 0)
                wstaw_pracownikow_do_bazy(dane);
            else if(zakladka == 1)
                wstaw_przedmioty_do_bazy(dane);
            else if(zakladka == 2)
                wstaw_grupy_do_bazy(dane);
            else if(zakladka == 3)
                wstaw_zjazdy_do_bazy(dane);
        }
    }

}

void MainWindow::on_pushButton_8_clicked()
{

    // Pobierz aktualną datę i czas i ustaw je jako przedrostek nazwy pliku
    QString nazwa_pliku = QDate::currentDate().toString("yyyyMMdd");
    nazwa_pliku += "_";
    nazwa_pliku += QTime::currentTime().toString("HHmmss");

    if(zakladka == 0)
        nazwa_pliku += "_pracownicy";
    else if(zakladka == 1)
        nazwa_pliku += "_przedmioty";
    else if(zakladka == 2)
        nazwa_pliku += "_grupy";
    else if(zakladka == 3)
        nazwa_pliku += "_zjazdy";
    else if(zakladka == 4)
        nazwa_pliku += "_przydzialy";
    nazwa_pliku += ".csv";

    QString sciezka = QFileDialog::getSaveFileName(this,"Zapisz plik z danymi",nazwa_pliku,"Pliki tekstowe (*.txt *.csv)");
    if(!sciezka.isNull() && czy_polaczono_z_baza)
    {
        if(zakladka < 4)
            zapisz_dane_do_pliku(sciezka);
        else
            zapisz_przydzialy(sciezka);
    }
}

QVector<QStringList> MainWindow::wczytaj_dane_z_pliku(QString sciezka)
{
    QFile plik(sciezka);
    QVector<QStringList> dane;

    if(plik.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        while(!plik.atEnd())
        {
            QString wiersz;
            wiersz = static_cast<QString>(plik.readLine()).remove(QRegExp("[\\n\\t\\r]"));
            //qDebug()<<wiersz;
            dane.append(wiersz.split(';'));
            //qDebug()<<dane;
        }
        plik.close();
    }
    else
    {
        qDebug()<<"Błąd otwarcia pliku";
    }
    return dane;

}

void MainWindow::zapisz_dane_do_pliku(QString sciezka)
{
    QFile plik(sciezka);
    QVector<QStringList> dane;
    QSqlTableModel *tmp_tab = pracownicy;

    QSqlRecord wiersz;

    if(zakladka == 0)
        tmp_tab = pracownicy;
    else if(zakladka == 1)
        tmp_tab = program_studiow;
    else if(zakladka == 2)
        tmp_tab = grupy;
    else if(zakladka == 3)
        tmp_tab = zjazdy;
    int liczba_pol = tmp_tab->record().count();
    //qDebug()<<tmp_tab->rowCount();
    if(plik.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&plik);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(false);
        for(int i = 0; i < tmp_tab->rowCount(); i++)
        {
            wiersz = tmp_tab->record(i);
            QString napis="";
            for(int j=1; j<liczba_pol; j++) //nie odczytujemy pola id
            {
                QString pole = wiersz.field(j).value().toString();
                napis += pole;
                napis += ";";
            }
            napis.remove(napis.length()-1,1);
            //napis += "\n";
            out<<napis<<endl;
        }
        plik.close();

    }
}

void MainWindow::zapisz_przydzialy(QString sciezka)
{
    QFile plik(sciezka);

    QSqlRecord wiersz;
    QSqlQuery zapytanie1, zapytanie2, zapytanie3;
    QString ss = "SELECT id_pracownika, id_przedmiotu, grupa FROM przydzial_zajec";
    zapytanie2.exec(ss);

    //qDebug()<<tmp_tab->rowCount();
    if(plik.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&plik);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(false);
        //for(int i = 0; i < przydzialy->rowCount(); i++)
        while(zapytanie2.next())
        {
            //wiersz = przydzialy->record(i);
            QString napis="";
            QString id_pracownika = zapytanie2.value(0).toString();
            QString id_przedmiotu = zapytanie2.value(1).toString();
            QString id_grupy = zapytanie2.value(2).toString();
            //QString do_zaplanowania = wiersz.field(4).value().toString();

            QString tmp = "SELECT rok_studiow, nazwa_przedmiotu, typ_zajec, liczba_godzin FROM programy_studiow WHERE id = " + id_przedmiotu;
            //qDebug()<<tmp;
            zapytanie1.exec(tmp);
            zapytanie1.next();
            napis += zapytanie1.value(0).toString().simplified();
            napis += ";";
            napis += zapytanie1.value(1).toString().simplified();
            napis += ";";
            napis += zapytanie1.value(3).toString().simplified();
            napis += ";";
            napis += zapytanie1.value(2).toString().simplified();
            napis += ";";
            QString tmp2 = "SELECT nazwa FROM grupy WHERE id = " + id_grupy;
            zapytanie1.exec(tmp2);
            zapytanie1.next();
            napis += zapytanie1.value(0).toString().simplified();
            napis += ";";
            QString tmp3 = "SELECT imie, nazwisko, stopien FROM pracownicy WHERE id = " + id_pracownika;
            zapytanie3.exec(tmp3);
            zapytanie3.next();
            napis += zapytanie3.value(2).toString().simplified();
            napis += ";";
            napis += zapytanie3.value(0).toString().simplified();
            napis += ";";
            napis += zapytanie3.value(1).toString().simplified();

            out<<napis<<endl;


        }
        plik.close();

    }
}


void MainWindow::zapisz_plan(QString sciezka)
{
    QFile plik(sciezka);

    QSqlRecord wiersz;
    QSqlQuery zapytanie1, zapytanie2, zapytanie3, zapytanie4, zapytanie5;
    QString ss = "SELECT dzien, id_przydzialu, od_godzina, do_godzina, sala, id_grupy FROM plan_zajec ORDER BY id_grupy, dzien, od_godzina";
    zapytanie2.exec(ss);


    if(plik.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&plik);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(false);

        while(zapytanie2.next())
        {

            QString napis="";
            QString id_dzien = zapytanie2.value(0).toString();
            QString id_przydzialu = zapytanie2.value(1).toString();
            QString od_godzina = zapytanie2.value(2).toString();
            QString do_godzina = zapytanie2.value(3).toString();
            QString id_grupy = zapytanie2.value(5).toString();
           // qDebug()<<id_przydzialu;
         if(id_przydzialu != "0")
         {
             QString tmp4 = "SELECT id_pracownika, id_przedmiotu FROM przydzial_zajec WHERE id = "+id_przydzialu;
             zapytanie4.exec(tmp4);
             zapytanie4.next();
             QString id_pracownika = zapytanie4.value(0).toString();
             QString id_przedmiotu = zapytanie4.value(1).toString();



             QString tmp5 = "SELECT rok_studiow, nazwa_przedmiotu FROM programy_studiow WHERE id = " + id_przedmiotu;
             //qDebug()<<tmp;
             zapytanie5.exec(tmp5);
             zapytanie5.next();

             napis += "Rok ";
             napis += zapytanie5.value(0).toString().simplified();
             napis += ";";





             QString tmp2 = "SELECT nazwa FROM grupy WHERE id = " + id_grupy;
            zapytanie1.exec(tmp2);
            zapytanie1.next();
            napis += zapytanie1.value(0).toString().simplified();
            napis += ";";

            QString tmp0 = "SELECT data FROM zjazdy WHERE id = " + id_dzien;
            zapytanie1.exec(tmp0);
            zapytanie1.next();
            napis += zapytanie1.value(0).toString().simplified();
            napis += ";";

            napis+=od_godzina;
            napis += ":00-";
            napis+=do_godzina;
            napis += ":00;";

            napis += zapytanie5.value(1).toString().simplified();
            napis += ";";


            QString tmp3 = "SELECT imie, nazwisko, stopien FROM pracownicy WHERE id = " + id_pracownika;
            zapytanie3.exec(tmp3);
            zapytanie3.next();
            napis += zapytanie3.value(2).toString().simplified();
            napis += ";";
            napis += zapytanie3.value(0).toString().simplified();
            napis += ";";
            napis += zapytanie3.value(1).toString().simplified();
            //qDebug()<<napis;
            out<<napis<<endl;

          }
        }
        plik.close();

    }
}


void MainWindow::wstaw_pracownikow_do_bazy(QVector<QStringList> dane)
{
    QSqlQuery zapytanie;
    QString s = "INSERT INTO pracownicy(imie,nazwisko,stopien,stanowisko) VALUES";
    for(int i=0; i<dane.count(); i++)
    {
        s += "('";
        s += dane.at(i).at(0);
        s += "','";
        s += dane.at(i).at(1);
        s += "','";
        s += dane.at(i).at(2);
        s += "','";
        s += dane.at(i).at(3);
        s += "'),";
    }
    s.remove(s.length()-1,1); //usuwamy przecinek z końca
    //qDebug()<<s;
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(s);
        pracownicy->select();
    }
}

void MainWindow::wstaw_przedmioty_do_bazy(QVector<QStringList> dane)
{
    QSqlQuery zapytanie;
    QString s = "INSERT INTO programy_studiow(rok_studiow,nazwa_przedmiotu,liczba_godzin,typ_zajec) VALUES";
    for(int i=0; i<dane.count(); i++)
    {
        s += "(";
        s += dane.at(i).at(0);
        s += ",'";
        s += dane.at(i).at(1);
        s += "',";
        s += dane.at(i).at(2);
        s += ",'";
        s += dane.at(i).at(3);
        s += "'),";
    }
    s.remove(s.length()-1,1); //usuwamy przecinek z końca
    //qDebug()<<s;
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(s);
        program_studiow->select();
    }
}

void MainWindow::wstaw_grupy_do_bazy(QVector<QStringList> dane)
{
    QSqlQuery zapytanie;
    QString s = "INSERT INTO grupy(rok_studiow,nazwa) VALUES";
    for(int i=0; i<dane.count(); i++)
    {
        s += "(";
        s += dane.at(i).at(0);
        s += ",'";
        s += dane.at(i).at(1);
        s += "'),";
    }
    s.remove(s.length()-1,1); //usuwamy przecinek z końca
    //qDebug()<<s;
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(s);
        grupy->select();
    }
}

void MainWindow::wstaw_zjazdy_do_bazy(QVector<QStringList> dane)
{
    QSqlQuery zapytanie;
    QString s = "INSERT INTO zjazdy(data) VALUES";
    for(int i=0; i<dane.count(); i++)
    {
        s += "('";
        s += dane.at(i).at(0);
        s += "'),";
    }
    s.remove(s.length()-1,1); //usuwamy przecinek z końca
    //qDebug()<<s;
    if(czy_polaczono_z_baza)
    {
        zapytanie.exec(s);
        zjazdy->select();
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ustaw_szerokosc_kolumn();
    if(index==0) //pracownicy
    {
        zakladka = 0;
        ui->pushButton_5->setText("Dodaj pracownika");
        ui->pushButton_6->setText("Usuń pracownika");
        ui->tab->setAutoFillBackground(true);
    }
    else if(index==1) //przedmioty
    {
        zakladka = 1;
        ui->pushButton_5->setText("Dodaj przedmiot");
        ui->pushButton_6->setText("Usuń przedmiot");
        ui->tab_2->setAutoFillBackground(true);
    }
    else if(index==2) //grupy
    {
        zakladka = 2;
        ui->pushButton_5->setText("Dodaj grupę");
        ui->pushButton_6->setText("Usuń grupę");
        ui->tab_4->setAutoFillBackground(true);
    }
    else if(index==3) //zjazdy
    {
        zakladka = 3;
        ui->pushButton_5->setText("Dodaj zjazd");
        ui->pushButton_6->setText("Usuń zjazd");
        ui->tab_5->setAutoFillBackground(true);
    }
    else if(index==4) //przydziały
    {
        zakladka = 4;
        ui->pushButton_5->setText("Dodaj przydział");
        ui->pushButton_6->setText("Usuń przydział");
        ui->pushButton_7->setVisible(false);
        ui->tab_3->setAutoFillBackground(true);
    }
}

// Filtrowanie wyświetlanych danych

void MainWindow::ustaw_filtr()
{
    QString filtr="";
    if(zakladka == 0)  //prowadzacy
    {
        if(!filtr_stopien.isEmpty())
            filtr = filtr + "stopien=" + filtr_stopien;

        if(!filtr_zatrudnienie.isEmpty() && !filtr.isEmpty())
            filtr = filtr + " AND stanowisko=" + filtr_zatrudnienie;

        if(!filtr_zatrudnienie.isEmpty() && filtr.isEmpty())
            filtr = filtr + "stanowisko=" + filtr_zatrudnienie;
        //qDebug()<<filtr;
        pracownicy->setFilter(filtr);
       // qDebug()<<pracownicy->query().lastInsertId();
    }
    else if(zakladka == 1)  //przedmioty
    {
        if(!filtr_rok_studiow.isEmpty())
            filtr = filtr + "rok_studiow=" + filtr_rok_studiow;

        if(!filtr_typ_zajec.isEmpty() && !filtr.isEmpty())
            filtr = filtr + " AND typ_zajec=" + filtr_typ_zajec;

        if(!filtr_typ_zajec.isEmpty() && filtr.isEmpty())
            filtr = filtr + "typ_zajec=" + filtr_typ_zajec;
        //qDebug()<<filtr;
        program_studiow->setFilter(filtr);
    }

}




void MainWindow::on_checkBox_clicked(bool checked)
{
    czy_rok_aktywny[1] = checked;
    zakryj_opcje_grup();
    //qDebug()<<czy_rok_aktywny[1];
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    czy_rok_aktywny[2] = checked;
    zakryj_opcje_grup();
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    czy_rok_aktywny[3] = checked;
    zakryj_opcje_grup();
}

void MainWindow::on_checkBox_4_clicked(bool checked)
{
    czy_rok_aktywny[4] = checked;
    zakryj_opcje_grup();
}

void MainWindow::zakryj_opcje_grup()
{
    if(czy_rok_aktywny[1])
    {
        ui->label_2->setEnabled(true);
        ui->label_3->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
    }
    else
    {
        ui->label_2->setEnabled(false);
        ui->label_3->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
    }

    if(czy_rok_aktywny[2])
    {
        ui->label_4->setEnabled(true);
        ui->label_5->setEnabled(true);
        ui->spinBox_3->setEnabled(true);
        ui->spinBox_4->setEnabled(true);
    }
    else
    {
        ui->label_4->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
        ui->spinBox_4->setEnabled(false);
    }

    if(czy_rok_aktywny[3])
    {
        ui->label_6->setEnabled(true);
        ui->label_7->setEnabled(true);
        ui->label_8->setEnabled(true);
        ui->spinBox_5->setEnabled(true);
        ui->spinBox_6->setEnabled(true);
        ui->spinBox_7->setEnabled(true);
    }
    else
    {
        ui->label_6->setEnabled(false);
        ui->label_7->setEnabled(false);
        ui->label_8->setEnabled(false);
        ui->spinBox_5->setEnabled(false);
        ui->spinBox_6->setEnabled(false);
        ui->spinBox_7->setEnabled(false);
    }

    if(czy_rok_aktywny[4])
    {
        ui->label_9->setEnabled(true);
        ui->label_10->setEnabled(true);
        ui->label_11->setEnabled(true);
        ui->spinBox_8->setEnabled(true);
        ui->spinBox_9->setEnabled(true);
        ui->spinBox_10->setEnabled(true);
    }
    else
    {
        ui->label_9->setEnabled(false);
        ui->label_10->setEnabled(false);
        ui->label_11->setEnabled(false);
        ui->spinBox_8->setEnabled(false);
        ui->spinBox_9->setEnabled(false);
        ui->spinBox_10->setEnabled(false);
    }
}

//resetuj (usuń) grupy
void MainWindow::on_pushButton_10_clicked()
{
    if(QMessageBox::question(this,"Potwierdzenie","Czy na pewno chcesz usunąć wszystkie grupy?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        resetuj_grupy();
        //grupy->select();
    }

}

void MainWindow::resetuj_grupy()
{
    if(czy_polaczono_z_baza)
    {
        //to nie chce działać :-(
        //grupy->removeRows(0,grupy->rowCount()-1);

        for(int i=0; i<grupy->rowCount(); i++)
            grupy->removeRow(i);
        grupy->select();
    }

}

//generuj grupy
void MainWindow::on_pushButton_9_clicked()
{
   QSqlQuery zapytanie;
   QString s;
   odczytaj_liczbe_grup();
   for(int i=1; i<=4; i++)
   {
       if(czy_rok_aktywny[i])
       {
           s = "INSERT INTO grupy(rok_studiow, nazwa) VALUES (";
           s += QString::number(i);
           s += ",'wyk')";
           //qDebug()<<s;
           zapytanie.exec(s);
           for(int j=1; j<=gr_cw[i]; j++)
           {
               s = "INSERT INTO grupy(rok_studiow, nazwa) VALUES (";
               s += QString::number(i);
               s += ",'gr";
               s += QString::number(j);
               s += "')";
               zapytanie.exec(s);
               //qDebug()<<s;
           }
           for(int j=1; j<=gr_lab[i]; j++)
           {
               s = "INSERT INTO grupy(rok_studiow, nazwa) VALUES (";
               s += QString::number(i);
               s += ",'lab";
               s += QString::number(j);
               s += "')";
               zapytanie.exec(s);
               //qDebug()<<s;
           }
           if(i>2)
               for(int j=1; j<=gr_sem[i]; j++)
               {
                   s = "INSERT INTO grupy(rok_studiow, nazwa) VALUES (";
                   s += QString::number(i);
                   s += ",'sem";
                   s += QString::number(j);
                   s += "')";
                   zapytanie.exec(s);
                   //qDebug()<<s;
               }
       }
   }
   qDebug()<< db.lastError().databaseText()<<endl;
   grupy->select();
}

void MainWindow::odczytaj_liczbe_grup()
{
    gr_cw[1] = ui->spinBox->value();
    gr_cw[2] = ui->spinBox_4->value();
    gr_cw[3] = ui->spinBox_5->value();
    gr_cw[4] = ui->spinBox_8->value();

    gr_lab[1] = ui->spinBox_2->value();
    gr_lab[2] = ui->spinBox_3->value();
    gr_lab[3] = ui->spinBox_6->value();
    gr_lab[4] = ui->spinBox_9->value();

    gr_sem[3] = ui->spinBox_7->value();
    gr_sem[4] = ui->spinBox_10->value();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    gr_cw[1] = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    gr_lab[1] = arg1;
}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    gr_cw[2] = arg1;
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    gr_lab[2] = arg1;
}

void MainWindow::on_spinBox_5_valueChanged(int arg1)
{
    gr_cw[3] = arg1;
}

void MainWindow::on_spinBox_6_valueChanged(int arg1)
{
    gr_lab[3] = arg1;
}

void MainWindow::on_spinBox_7_valueChanged(int arg1)
{
    gr_sem[3] = arg1;
}

void MainWindow::on_spinBox_8_valueChanged(int arg1)
{
    gr_cw[4] = arg1;
}

void MainWindow::on_spinBox_9_valueChanged(int arg1)
{
    gr_lab[4] = arg1;
}

void MainWindow::on_spinBox_10_valueChanged(int arg1)
{
    gr_sem[4] = arg1;
}

// Obsługa przycisków radiowych filtrujących dane

void MainWindow::on_radioButton_2_clicked()
{
    filtr_rok_studiow = "1";
    ustaw_filtr();
}

void MainWindow::on_radioButton_4_clicked()
{
    filtr_rok_studiow = "2";
    ustaw_filtr();
}

void MainWindow::on_radioButton_clicked()
{
    filtr_rok_studiow = "3";
    ustaw_filtr();
}

void MainWindow::on_radioButton_3_clicked()
{
    filtr_rok_studiow = "4";
    ustaw_filtr();
}

void MainWindow::on_radioButton_5_clicked()
{
    filtr_rok_studiow = "";
    ustaw_filtr();
}

void MainWindow::on_radioButton_6_clicked()
{
    filtr_typ_zajec = "'w'"; //wyklady
    ustaw_filtr();
}

void MainWindow::on_radioButton_7_clicked()
{
    filtr_typ_zajec = "'ć'"; //ćwiczenia
    ustaw_filtr();
}

void MainWindow::on_radioButton_8_clicked()
{
    filtr_typ_zajec = "'l'"; //laboratoria
    ustaw_filtr();
}

void MainWindow::on_radioButton_10_clicked()
{
    filtr_typ_zajec = "'s'"; //seminaria
    ustaw_filtr();
}

void MainWindow::on_radioButton_9_clicked()
{
    filtr_typ_zajec = ""; //wszystko
    ustaw_filtr();
}

void MainWindow::on_radioButton_11_clicked()
{
    filtr_stopien = "'mgr'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_12_clicked()
{
    filtr_stopien = "'dr'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_13_clicked()
{
    filtr_stopien = "'dr hab.'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_14_clicked()
{
    filtr_stopien = "'prof. dr hab.'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_15_clicked()
{
    filtr_stopien = ""; //wszystko
    ustaw_filtr();
}

void MainWindow::on_radioButton_16_clicked()
{
    filtr_zatrudnienie = "'nauczyciel - UMK'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_17_clicked()
{
    filtr_zatrudnienie = "'inny - UMK'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_18_clicked()
{
    filtr_zatrudnienie = "'doktorant'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_19_clicked()
{
    filtr_zatrudnienie = "'spoza UMK'";
    ustaw_filtr();
}

void MainWindow::on_radioButton_20_clicked()
{
    filtr_zatrudnienie = ""; //wszyscy
    ustaw_filtr();
}


void MainWindow::on_calendarWidget_selectionChanged()
{
    QDate data = ui->calendarWidget->selectedDate();
    //qDebug()<<data;
    QSqlQuery zapytanie;
    QString s;
    if(czy_polaczono_z_baza)
    {
        s = "INSERT INTO zjazdy(data) VALUES ('";
        s += data.toString(Qt::ISODate);
        s += "')";
        //qDebug()<<s;
        zapytanie.exec(s);
        qDebug()<< db.lastError().databaseText()<<endl;
        zjazdy->select();
    }

}

void MainWindow::on_tableView_4_clicked(const QModelIndex &index)
{
    dodaj_godziny_do_przydzialow();
}

void MainWindow::on_pushButton_11_clicked()
{
    for(int i =1; i<5; i++)
        if(gen_przydzialy[i]) generuj_przydzialy(i);
}

void MainWindow::on_checkBox_5_clicked(bool checked)
{
    gen_przydzialy[1] = checked;
}

void MainWindow::on_checkBox_6_clicked(bool checked)
{
    gen_przydzialy[2] = checked;
}

void MainWindow::on_checkBox_7_clicked(bool checked)
{
    gen_przydzialy[3] = checked;
}

void MainWindow::on_checkBox_8_clicked(bool checked)
{
    gen_przydzialy[4] = checked;
}
