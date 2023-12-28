#include "gamehandler.h"

GameHandler::GameHandler():QObject()
{
    mLevel = 5;
    hostID = 0;
}

GameHandler::~GameHandler()
{
    delete controller;
}

bool GameHandler::addPlayer(qintptr playerId)
{
    if(hostID == 0){

        hostID = playerId;

    }
    if(playerList.size() == 10){

        return false;

    }
    else{

        playerList.append(playerId);
        return true;
    }
}

QList<qintptr> GameHandler::getPlayerList()
{
    return playerList;
}

bool GameHandler::startGame()
{
    if(playerList.size() < 2){

        return false;
    }


    else{
        for (int i = 0; i < playerList.size();i++){
            emit gameStarting(playerList[i]);
        }
        QList<int> cards;

        for (int i = 0; i < playerList.size();i++){
            int level = 0;
            while(level < mLevel){


                int newCard = qrand()%100;
                if(!cards.contains(newCard)){

                    cards.append(newCard);
                    level++;
                }
            }


        }
        controller = new Controller(cards);




        for (int i = 0; i < playerList.size();i++){



            for(int level =0; level < mLevel;level++){

                emit sendCard(playerList[i],cards[(mLevel * i)+level]);

            }

            howMuchCards[playerList[i]] = mLevel;

        }

        return true;


    }
}



int GameHandler::getLevel()
{
    return mLevel;
}

void GameHandler::leaveGame(qintptr playerID)
{
    if(playerID == hostID){
        emit deleteMe();
    }
    else{
        this->playerList.removeAt(playerList.indexOf(playerID));
    }
}

void GameHandler::nextPlayer(int idxOfPlayer)
{
    if(controller->numberOfCards() == 0){

        emit gameWon(playerList);
    }
    else if(idxOfPlayer + 1 == playerList.size()){

        if(howMuchCards[playerList[idxOfPlayer]] == 0){
            nextPlayer(0);
        }
        else{
            emit turnChanged(playerList[0]);
        }
    }
    else{
        idxOfPlayer += 1;

        if(howMuchCards[playerList[idxOfPlayer]] == 0){
            nextPlayer(idxOfPlayer);
        }
        else{
            emit turnChanged(playerList[idxOfPlayer]);
        }
    }
}

void GameHandler::addCard(qintptr playerID,int cardNumber)
{
    if(cardNumber == -1){

        nextPlayer(playerList.indexOf(playerID));

    }
    else{

        if(controller->cardAdded(cardNumber)){
            nextPlayer(playerList.indexOf(playerID));


        }
        else{

            for (int i = 0; i < playerList.size(); i++){


                emit gameOver(playerList[i]);


            }
            emit deleteMe();


        }
    }
}

void GameHandler::selectLevel(int level)
{
    mLevel = level;
}


