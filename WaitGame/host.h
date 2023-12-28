#ifndef HOST_H
#define HOST_H

#include <QTcpServer>
#include <QObject>

#include "sockets.h"
#include "gamehandler.h"
#include <QtGlobal>


class Host: public QTcpServer
{
    Q_OBJECT
public:
    Host();
    ~Host();

public slots:
    void newGame(qintptr playerID);
    void startGame(qintptr);
    void sendCard(qintptr, int);
    void gameStarting(qintptr);
    void gameOver(qintptr);
    void joinGame(qintptr,qintptr);
    void getGames(qintptr);
    void gameWon(QList<qintptr>);
    void changeLevel(qintptr,int);
    void addCard(qintptr playerID,qintptr hostID, int cardNo);

    //not done
    void getPlayers(qintptr playerID,qintptr gameID);
    void leaveLobby(qintptr playerID, qintptr gameID);

private:
    QMap<qintptr,bool> socketFree;
    QMap<qintptr,Socket*> mSocketList;
    QMap<qintptr,GameHandler*> mGames;
    virtual void incomingConnection(qintptr handle) override;






};

#endif // HOST_H
