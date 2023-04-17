#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>


class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client();


public slots:
    void connectToServer(QString host, quint16 port);
    void sendMessage(QString message);

private slots:
    void readData();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void messageReceived(const QString &message);

private:
    QTcpSocket *socket;
};


#endif // CLIENT_H
