#include "sockets.h"

Socket::Socket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));


}




void Socket::readMessage()
{
    QStringList data = QString(readAll()).split(" ");

    if(data[0] ==  QString("newGame")){

        emit newGame(this->socketDescriptor());
    }
    else if(data[0] == QString("startGame")){

        emit startGame(socketDescriptor());
    }
    else if(data[0] == QString("joinGame")){

        emit joinGame(socketDescriptor(),data[1].toULongLong());
    }
    else if(data[0] == QString("getGames")){
        emit getGames(socketDescriptor());
    }
    else if(data[0] == QString("level")){

        emit changeLevel(socketDescriptor(),data[1].toInt());
    }
    else if(data[0] == QString("addCard")){

        emit addCard(socketDescriptor(),data[1].toInt(),data[2].toInt());
    }
    else if (data[0] == QString("getPlayers")){
        emit getPlayers(socketDescriptor(),data[1].toInt());
    }
    else if (data[0] == QString("leaveLobby")){
        emit leaveLoby(socketDescriptor(),data[1].toInt());
    }




    qDebug() << data;




}
