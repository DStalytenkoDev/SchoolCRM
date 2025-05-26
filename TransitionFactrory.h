#ifndef TRANSITIONFACTRORY_H
#define TRANSITIONFACTRORY_H


#include <QSignalTransition>
#include <QState>


template<typename SenderType, typename Signal, typename ReciverType, typename Slot>
void transition(SenderType sender, Signal signal, ReciverType reciver, Slot slot, QState* parentState)
{
    auto transition = new QSignalTransition(sender, signal);

    QObject::connect(transition, &QSignalTransition::triggered, reciver, slot);

    parentState->addTransition(transition);
}


template<typename SenderType, typename Signal, typename ReciverType, typename Slot>
void transition(SenderType sender, Signal signal, ReciverType reciver, Slot slot, QState* parentState, QState* finalState)
{
    auto transition = parentState->addTransition(sender, signal, finalState);

    QObject::connect(transition, &QSignalTransition::triggered, reciver, slot);
}


#endif // TRANSITIONFACTRORY_H
