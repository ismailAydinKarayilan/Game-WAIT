#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>

class Controller
{
public:
    explicit Controller(QList<int> &cards);

    bool cardAdded(int cardNumber);

    int numberOfCards();

private:
    QList<int> mCards;
};

#endif // CONTROLLER_H
