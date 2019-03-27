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
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString query = ui->textEdit->toPlainText();

    QMessageBox::information(this, "Zapytanie", "Wysłano zapytanie: \n" + query);
    ui->statusBar->showMessage(query, 2000);

    QSqlQuery zapytanie;
    zapytanie.first(); //nie dzila nikt nie wie dlaczego
    QString wynik = zapytanie.value(0).toString(); //to tez
    qDebug() << wynik;
    //QMessageBox::information(this, "", "Wynik zapytania: \n" + wynik);


}

void MainWindow::on_pushButton_3_clicked()
{
    db.setHostName("serwer1866993.home.pl");
    db.setDatabaseName("28568274_postgres");
    db.setUserName("28568274_postgres");
    db.setPassword("!QAZXSW@2wsxzaq1");

    if(db.open()){
        qDebug() << "Opened connection!";
        ui->textEdit->setText("SELECT od_godzina FROM plan_zajec");

    }

    else qDebug() << "Something went wrong D:";



}

void MainWindow::on_pushButton_2_clicked()
{
    db.close();
    qDebug() << "And closed";
}
