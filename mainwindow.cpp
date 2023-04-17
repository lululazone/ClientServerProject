#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
   ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_DbConnectButton_clicked()
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setUserName("root");
//    db.setPassword("");
//    db.setDatabaseName("qtdb");
//    QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
       db.setDatabaseName("./qtdb.db");
    if (db.open()) {
         QMessageBox::information(this, "Connection", "Db Connected Successfully");
    } else {
        QString error = db.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to connect to database: " + error);
    }
}

