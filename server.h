#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QTcpServer>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
    QThread thread;

public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void startServer(quint16 port);
    void sendMessage(QString message);
    void readData();
    void scanDisk();
    void threaded();

private slots:
    void newConnection();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void messageReceived(const QString &message);

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
};


#endif // SERVER_H
