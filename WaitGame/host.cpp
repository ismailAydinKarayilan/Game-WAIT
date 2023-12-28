#include "host.h"



Host::Host(): QTcpServer()
{
    listen(QHostAddress::LocalHost,1234);



}

Host::~Host()
{
    for (int i = 0;i< mSocketList.keys().size();i++){

        delete mSocketList[mSocketList.keys()[i]];
    }
    for (GameHandler* i : mGames){
        delete i;
    }
}

void Host::newGame(qintptr playerID)
{
    mGames[playerID] = new GameHandler();
    mGames[playerID]->addPlayer(playerID);
    connect(mGames[playerID],SIGNAL(gameStarting(qintptr)),this,SLOT(gameStarting(qintptr)));
    connect(mGames[playerID],SIGNAL(sendCard(qintptr,int)),this,SLOT(sendCard(qintptr,int)));
    connect(mGames[playerID],SIGNAL(gameOver(qintptr)),this,SLOT(gameOver(qintptr)));
    connect(mGames[playerID],&GameHandler::gameWon,this,&Host::gameWon);
    connect(mGames[playerID],&GameHandler::deleteMe,this,[=](){
        delete mGames[playerID];
        mGames.remove(playerID);
    });



    while(socketFree[playerID]){

    }
    socketFree[playerID] = true;
    mSocketList[playerID]->write("New game created");
    mSocketList[playerID]->flush();
    socketFree[playerID] = false;


    //TO DO CONNECTIONS
}

void Host::startGame(qintptr playerID)
{
    if(!mGames[playerID]->startGame()){
        while(socketFree[playerID]){

        }
        socketFree[playerID] = true;
        mSocketList[playerID]->write("error1");
        mSocketList[playerID]->flush();
        socketFree[playerID] = false;
    }
    else{
        QList<qintptr> playersStarted = mGames[playerID]->getPlayerList();

        for(int i = 0 ; i < playersStarted.size();i++){
            while(socketFree[playersStarted[i]]){

            }
            socketFree[playersStarted[i]] = true;
            mSocketList[playersStarted[i]]->write("gameStarted");
            mSocketList[playersStarted[i]]->flush();
            mSocketList[playersStarted[i]]->waitForBytesWritten(1000);
            socketFree[playersStarted[i]] = false;
        }
    }
}

void Host::sendCard(qintptr playerID, int cardNo)
{
    while(socketFree[playerID]){

    }
    socketFree[playerID] = true;
    mSocketList[playerID]->write(QString(QString("newCard ")+QString::number(cardNo)).toUtf8());
    mSocketList[playerID]->flush();

    mSocketList[playerID]->waitForBytesWritten(10000);
    socketFree[playerID] = false;
}

void Host::gameStarting(qintptr playerID)
{
    while(socketFree[playerID]){

    }
    socketFree[playerID] = true;
    mSocketList[playerID]->write("gameStarting");
    mSocketList[playerID]->flush();
    mSocketList[playerID]->waitForBytesWritten(1000);
    socketFree[playerID] = false;
}

void Host::gameOver(qintptr playerID)
{
    while(socketFree[playerID]){

    }
    socketFree[playerID] = true;
    mSocketList[playerID]->write("gameOver");
    mSocketList[playerID]->flush();
    socketFree[playerID] = false;
}

void Host::joinGame(qintptr playerID, qintptr gameID)
{
    mGames[gameID]->addPlayer(playerID);
}

void Host::getGames(qintptr playerID)
{
    QList<qintptr> gameKeys = mGames.keys();
    QString message = "";

    for (int i = 0; i < gameKeys.size(); ++i) {

        message += QString(" ") + QString::number(gameKeys[i]);
    }

    while(socketFree[playerID]){

    }
    socketFree[playerID] = true;
    mSocketList[playerID]->write(QString(QString("gamelist")+message).toUtf8());
    mSocketList[playerID]->flush();
    socketFree[playerID] = false;

}

void Host::gameWon(QList<qintptr> playerList)
{
    for (qintptr playerID : playerList){

        mSocketList[playerID]->write("gameWon");
    }
    delete mGames[playerList[0]];
}

void Host::changeLevel(qintptr hostID,int level)
{
    mGames[hostID]->selectLevel(level);
}

void Host::addCard(qintptr playerID, qintptr hostID, int cardNo)
{
    mGames[hostID]->addCard(playerID,cardNo);
}

void Host::getPlayers(qintptr playerID, qintptr gameID)
{
    QString message = "playerList:";
    QList<qintptr> playerListToSend = mGames[gameID]->getPlayerList();
    for (int i = 0 ; i < playerListToSend.size();i++){
        message += QString(" ") + QString::number(playerListToSend[i]);
    }
    message += QString(" ENDOFMESSAGE");
    mSocketList[playerID]->write(message.toUtf8());
}

void Host::leaveLobby(qintptr playerID, qintptr gameID)
{
    mGames[gameID]->leaveGame(playerID);

}

void Host::incomingConnection(qintptr handle)
{
    Socket* newSocket = new Socket();
    newSocket->setSocketDescriptor(handle);

    mSocketList[handle] = newSocket;
    socketFree[handle] = false;
    //TO DO CONNECTIONS
    connect(newSocket,SIGNAL(newGame(qintptr)),
                     this,SLOT(newGame(qintptr)));

    connect(newSocket,SIGNAL(startGame(qintptr)),
            this,SLOT(startGame(qintptr)));

    connect(newSocket,SIGNAL(joinGame(qintptr,qintptr)),
            this,SLOT(joinGame(qintptr,qintptr)));
    connect(newSocket,SIGNAL(getGames(qintptr)),
            this,SLOT(getGames(qintptr)));

    connect(newSocket, &Socket::changeLevel,
            this,&Host::changeLevel);
    connect(newSocket, &Socket::addCard,
            this,&Host::addCard);
    connect(newSocket,&Socket::getPlayers,
            this,&Host::getPlayers);
    connect(newSocket,&Socket::leaveLoby,
            this,&Host::leaveLobby);


    qDebug()<< "Connection coming";

}
