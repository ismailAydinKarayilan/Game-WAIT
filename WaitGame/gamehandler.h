#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H
#include <QObject>
#include <QMap>
#include "controller.h"

class GameHandler:public QObject
{

    Q_OBJECT
public:

    explicit GameHandler();
    ~GameHandler();

    bool addPlayer(int playerId);
    QList<qintptr> getPlayerList();
    bool startGame();
    int getLevel();
    void leaveGame(qintptr);

private:
    QMap<qintptr,int> howMuchCards;
    QList<qintptr> playerList;
    Controller* controller;
    int mLevel;
    qintptr hostID;


    void nextPlayer(int idxOfPlayer);


public:
signals:
    void sendCard(qintptr playerID,int cardNo);
    void turnChanged(qintptr nextPlayerId);
    void gameOver(qintptr playerID);
    void gameStarting(qintptr);
    void gameWon(QList<qintptr>);
    void deleteMe();


public slots:


    void addCard(qintptr playerID,int i);
    void selectLevel(int);

};

#endif // GAMEHANDLER_H
