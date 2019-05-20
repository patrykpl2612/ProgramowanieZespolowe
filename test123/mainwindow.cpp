#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    czy_polaczono_z_baza = false;
    filtr_rok_studiow = ""; //wszystkie
    filtr_typ_zajec = ""; //wszystkie

   ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableView, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(programy_menu( const QPoint& )));

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
  db.setHostName("localhost");
    db.setDatabaseName("plan_NSI");
    db.setUserName("postgres");
    db.setPassword("ketrab");

/*
    db.setHostName("serwer1866993.home.pl");
    db.setDatabaseName("28568274_postgres");
    db.setUserName("28568274_postgres");
    db.setPassword("!QAZXSW@2wsxzaq1");
*/
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
        ustaw_szerokosc_kolumn();
        ui->tableView->show();
       // ustaw_tabele();
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
    int szer = ui->tableView->width();

    ui->tableView->setColumnWidth(0, szer/20);
    ui->tableView->setColumnWidth(1, 4*szer/20);
    ui->tableView->setColumnWidth(2, 9*szer/20);
    ui->tableView->setColumnWidth(3, 4*szer/20);
    ui->tableView->setColumnWidth(4, 2*szer/20);

}

void MainWindow::ustaw_filtr()
{
    QString filtr="";

    if(!filtr_rok_studiow.isEmpty())
        filtr = filtr + "rok_studiow=" + filtr_rok_studiow;

    if(!filtr_typ_zajec.isEmpty() && !filtr.isEmpty())
        filtr = filtr + " AND typ_zajec=" + filtr_typ_zajec;

    if(!filtr_typ_zajec.isEmpty() && filtr.isEmpty())
        filtr = filtr + "typ_zajec=" + filtr_typ_zajec;

    program_studiow->setFilter(filtr);
    qDebug()<<program_studiow->query().lastInsertId();
}

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

void MainWindow::programy_menu( const QPoint& pos)
{
    QMenu *menu= new QMenu(this);
    QAction *usun = new QAction("Usuń przedmiot", this);
    QAction *dodaj = new QAction("Dodaj nowy przedmiot", this);
    connect(usun, SIGNAL(triggered()), this, SLOT(usunRekord()));
    connect(dodaj, SIGNAL(triggered()), this, SLOT(dodajRekord()));

    menu->addAction(dodaj);
    menu->addAction(usun);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

// Dodaj nowy rekord do tabeli programy studiów
void MainWindow::on_pushButton_5_clicked()
{
    dodajRekord();

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

void MainWindow::on_pushButton_6_clicked()
{
    usunRekord();
}

void MainWindow::usunRekord()
{
    QModelIndexList zaznaczone_wiersze = ui->tableView->selectionModel()->selectedRows();

    if(zaznaczone_wiersze.count() > 0)
    {
        if(QMessageBox::question(this,"Potwierdzenie","Czy na pewno chcesz usunąć te przedmioty?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            for(int i=0; i<zaznaczone_wiersze.count(); i++)
            {
                int zaznaczony_wiersz = zaznaczone_wiersze.at(i).row();
                program_studiow->removeRow(zaznaczony_wiersz);
            }
            program_studiow->select();
        }
    }
    else
    {
        QMessageBox::warning(this,"Błąd","Zaznacz wiersz, który chcesz usunąć", QMessageBox::Ok);
    }

}

void MainWindow::dodajRekord()
{
    QSqlQuery zapytanie;

    zapytanie.exec("INSERT INTO programy_studiow(rok_studiow,nazwa_przedmiotu, liczba_godzin, typ_zajec) VALUES (NULL,NULL,NULL,NULL)");
    qDebug()<< db.lastError().databaseText()<<endl;
    program_studiow->select();
}


void MainWindow::on_pushButton_7_clicked()
{
    QString sciezka = QFileDialog::getOpenFileName(this,"Otwórz plik z danymi","./","Pliki tekstowe (*.txt *.csv)");
    if(!sciezka.isNull())
    {
        QVector<QStringList> dane = wczytaj_dane_z_pliku(sciezka);
        if(!dane.isEmpty())
        {
            wstaw_przedmioty_do_bazy(dane);
        }
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
