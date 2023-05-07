#ifndef SERVER_H
#define SERVER_H

#include "Lexer.h"
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
    QFuture<void> future;
    QEventLoop loop;
    Lexer lexer;
    bool isStarted;



public slots:
    void startServer(quint16 port);
    void scanDisk();
    int countFiles(const QString& path);
    void readData();
    void getStatut();
    void sendMessageToClients(QString message);
    void onAsyncOperationFinished();
    void onAsyncOperationPause();
    void onAsyncOperationSuspend();
    void onAsyncOperationResumed();

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
