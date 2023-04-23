#include "mainwindow.h"
#include "errormanager.h"
#include "server.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
   ui->setupUi(this);
   server.startServer(7007);
   connect(&client, &Client::messageReceived, this, &MainWindow::appendMessage);
   client.connectToServer("localhost", 7007);
   client.sendMessage("Database init");
   ui->textEdit->insertPlainText("Wait for database to be init...");




}



MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::appendMessage(const QString& output)
{
   ui->textEdit->insertPlainText(output);
}



void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear();
    QString input;
    input = ui->lineEdit->text();
    client.sendMessage(input);
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
         QSqlDatabase db = QSqlDatabase::database("QSQLITE");
         QSqlQuery query;
         query.exec("CREATE TABLE IF NOT EXISTS 'files' ( \
         id INTEGER PRIMARY KEY AUTOINCREMENT, \
         filename TEXT NOT NULL, \
         last_modified DATETIME NOT NULL, \
         creation_date DATETIME NOT NULL, \
         max_size INTEGER NOT NULL, \
         min_size INTEGER NOT NULL, \
         size INTEGER NOT NULL, \
         ext TEXT NOT NULL, \
         type TEXT NOT NULL)");
        query.exec("INSERT INTO files (filename, last_modified, creation_date, max_size, min_size, size, ext, type) VALUES ('example.txt', '2022-03-15 13:45:00', '2022-01-01 09:00:00', 1000, 100, 500, 'txt', 'text/plain')");
    } else {
        QString error = db.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to connect to database: " + error);
    }

}

