//
// Created by javier on 2/11/19.
//

#ifndef MICROMACHINES_FINISHLOADINGSCREEN_H
#define MICROMACHINES_FINISHLOADINGSCREEN_H

#include "Command.h"

class FinishLoadingScreen : public Command{
public:
    FinishLoadingScreen(Scenario& scenario, Bot& bot) : Command(scenario, bot){}
    void apply() override{
        scenario.setLoadingScreen(false);
    }
};

#endif //MICROMACHINES_FINISHLOADINGSCREEN_H
