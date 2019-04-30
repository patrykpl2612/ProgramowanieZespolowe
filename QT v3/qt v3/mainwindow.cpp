#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include "qsqlerror.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->tableWidget->setMinimumSize(100,100);
    ui->scrollArea->setMinimumSize(100,100);

    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList godziny;
    for(int i=8; i<20; i++)
    {
        godziny.append(QString::number(i) + "-" + QString::number(i+1));
    }
    ui->tableWidget->setRowCount(godziny.length());
    ui->tableWidget->setVerticalHeaderLabels(godziny);

}


MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{

    db.setHostName("serwer1866993.home.pl");
    db.setDatabaseName("28568274_postgres");
    db.setUserName("28568274_postgres");
    db.setPassword("!QAZXSW@2wsxzaq1");

    if(db.open()){
        qDebug() << "Opened connection!";
        QSqlQuery zapytanie,zapytanie_2;

        QVBoxLayout * layout=new QVBoxLayout;
        ui->scrollAreaWidgetContents->setLayout(layout);
        ui->scrollArea->setWidgetResizable(true);

        zapytanie.exec("SELECT id_pracownika FROM przydzial_zajec");
        int licznik=0;
        while(zapytanie.next())
        {
            QString pracownik=zapytanie.value(0).toString();
            QString s="SELECT imie,nazwisko FROM pracownicy WHERE id = " + pracownik;
            zapytanie_2.exec(s);
            zapytanie_2.next();
            przyciski[licznik]=new QPushButton(zapytanie_2.value(0).toString() + "\n" + zapytanie_2.value(1).toString());
            przyciski[licznik]->setFixedSize(100,100);
            przyciski[licznik]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            layout->addWidget(przyciski[licznik]);
            licznik++;
        }
       QObject::connect(przyciski[0],SIGNAL (clicked()),this,SLOT(ustaw_przyciski()));

        zapytanie.exec("SELECT data FROM zjazdy");
        licznik=0;
        QStringList daty;
        while(zapytanie.next())
        {
            licznik++;
            daty.append(zapytanie.value(0).toString());
        }
        ui->tableWidget->setColumnCount(licznik);
        ui->tableWidget->setHorizontalHeaderLabels(daty);
    }

    else qDebug() << "Something went wrong :c";



}

void MainWindow::on_pushButton_2_clicked()
{
    db.close();
    qDebug() << "Closed";
}

void MainWindow::ustaw_przyciski()
{
    //przyciski[0]->setText("dzien dobry");
    QColor color(255,0,0);
    QString qss = QString("background-color: %1").arg(color.name());
    przyciski[0]->setStyleSheet(qss);
}
