#include "mainwindow.h"
#include "errormanager.h"
#include "ui_mainwindow.h"


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
    ui->textEdit->clear();
    QString input;
    ErrorManager error = *new ErrorManager();
    input = ui->lineEdit->text();
    Lexer lexer = *new Lexer();
    ui->textEdit->insertPlainText(lexer.Tokenize(input,error));
}

