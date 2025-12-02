// Copyright (C) 2025 Davyd Stalytenko
// SPDX-License-Identifier: GPL-3.0-only
//
// This file is part of SchooliesCave.
// 
// SchooliesCave is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation, version 3
// of the License only.
// 
// SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SchooliesCave.
// If not, see <https://www.gnu.org/licenses/>.


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
