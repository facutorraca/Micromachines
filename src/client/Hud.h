//
// Created by javif on 01/11/2019.
//

#ifndef MICROMACHINES_HUD_H
#define MICROMACHINES_HUD_H


#include "Camera.h"

class Hud {
    int32_t health;
    int32_t lap;
    int32_t total_laps;
    int32_t speed;
public:
    Hud();
    void draw(Camera& camera);
    void setHealth(int32_t h);
    void setSpeed(int32_t s);
    void setLap(int32_t lap);
    void setTotalLaps(int32_t laps);
};


#endif //MICROMACHINES_HUD_H
