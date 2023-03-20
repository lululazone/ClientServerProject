#include "sockettest.h"

SocketTest::SocketTest(QObject *parent) :
    QObject(parent)
{
}

void SocketTest::Connect()
{
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        //socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }




    // sent

    // got

    // closed
}



void SocketTest::SendMessage(QString message){

    if(socket){
        if(socket->isOpen()){
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(message.size()).toUtf8());
            header.resize(128);
            QByteArray byteArray = message.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;
        }
        else
                    qDebug() << "Socket is not Opened";
    }
    else
            qDebug() << "QTCPClient","Not connected";

}







void SocketTest::ServerListener(){
    while(true){
        qDebug() << "Reading: " << socket->bytesAvailable();
        qDebug() << socket->readAll();
    }
}
