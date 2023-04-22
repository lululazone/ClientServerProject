
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "server.h"
#include <QMainWindow>
#include <lexer.h>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Server server;
    Client client;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendMessage(const QString& output);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
