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
        QSqlQuery zapytanie;
 qDebug() <<"test1";
        zapytanie.exec("SELECT id FROM przydzial_zajec");
        int licznik=11;
        while(zapytanie.next())
        {
            qDebug() << zapytanie.value(0).toString();
            licznik++;
        }

        QVBoxLayout * layout=new QVBoxLayout;
        ui->scrollAreaWidgetContents->setLayout(layout);
        ui->scrollArea->setWidgetResizable(true);

        for(int i=0;i<licznik ;i++){
            przyciski[i]=new QPushButton(QString::number(i));
            przyciski[i]->setFixedSize(100,100);
            przyciski[i]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            layout->addWidget(przyciski[i]);

        }
    }

    else qDebug() << "Something went wrong :c";



}

void MainWindow::on_pushButton_2_clicked()
{
    db.close();
    qDebug() << "Closed";
}

