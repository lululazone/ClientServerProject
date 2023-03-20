#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = 0);

    void Connect();
    void ServerListener();
    void SendMessage(QString message);

signals:

public slots:

private:
    QTcpSocket *socket;

};

#endif // SOCKETTEST_H
