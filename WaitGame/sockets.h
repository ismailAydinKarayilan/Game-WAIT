#ifndef SOCKETS_H
#define SOCKETS_H

#include <QTcpSocket>
#include <QObject>

class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);

signals:
    void newGame(qintptr);
    void startGame(qintptr);
    void joinGame(qintptr,qintptr);
    void getGames(qintptr);
    void changeLevel(qintptr,int);
    void addCard(qintptr,qintptr,int);

    //not added
    void getPlayers(qintptr playerID,qintptr gameID);
    void leaveLoby(qintptr playerID,qintptr gameID);

private:



public slots:
    //message will be read here
    void readMessage();

};

#endif // SOCKETS_H
