#include "controller.h"
#include <QList>
Controller::Controller(QList<int> &cards)
{



    mCards = cards;

    std::sort(mCards.begin(),mCards.end());

}

bool Controller::cardAdded(int cardNumber)
{
    if(cardNumber == mCards.first()){


        mCards.pop_front();
        return true;
    }

    else{
        return false;
    }

}

int Controller::numberOfCards()
{
    return mCards.size();
}


