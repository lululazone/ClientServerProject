#ifndef SERVER_H
#define SERVER_H

#include "dbinteraction.h"
#include "qsqldatabase.h"
#include <QtNetwork>
#include <QTcpServer>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>


class Server : public QObject
{
    Q_OBJECT
    QThread thread;

public:
    explicit Server(QObject *parent = nullptr);
    DbInteraction dbManager;
    QString message;
    bool isPaused;
    bool isTerminated;
    bool isFinished;
    bool isStopped;
    bool isStarted;



public slots:
    void startServer(quint16 port);
    void scanDisk();

    void readData();
    void getStatut();
    void sendMessageToClients(QString message);

private slots:
    void newConnection();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void messageReceived(const QString &message);
    void sendMessage(QString message);

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
    void threaded();
};


#endif // SERVER_H
