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
   qDebug() <<output;
   if(outputToList.contains("percentage")){
       for(int j =0; j<outputToList.size();j++){

           if(outputToList[j] == "percentage"){
               if(j+1<outputToList.size()){
                   ui->progressBar->setValue(outputToList[j+1].toInt());
                   outputToList.removeAt(j+1);
               }
               if(j>=0){
                   outputToList.removeAt(j);
               }
           }
       }
       QString printable ="";
       for(int i=0;i<outputToList.size();i++){
           printable+=outputToList[i] + " ";
       }
       ui->textEdit->insertPlainText(printable);
   }
   else{
       if(output.contains("Error")){
               ui->textEdit->setTextColor(QColorConstants::Red);
       }
       else{
           ui->textEdit->setTextColor(QColorConstants::Black);
       }
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

