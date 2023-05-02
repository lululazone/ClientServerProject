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
   QStringList outputToList = output.split(" ");
   if(outputToList.contains("percentage")){
       ui->progressBar->setValue(outputToList[1].toInt());
   }
   else{
    ui->textEdit->insertPlainText(output);
   }

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

}

