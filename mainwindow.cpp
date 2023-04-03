#include "mainwindow.h"
#include "ui_mainwindow.h"

<<<<<<< HEAD
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
=======

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
   ui->setupUi(this);



>>>>>>> origin/UI
}

MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< HEAD
=======

void MainWindow::on_pushButton_clicked()
{

}

>>>>>>> origin/UI
