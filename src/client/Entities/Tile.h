//
// Created by javier on 24/10/19.
//

#ifndef MICROMACHINES_TILE_H
#define MICROMACHINES_TILE_H


#include <client/Entities/TileInfo.h>
#include <client/Camera.h>
#include "Entity.h"

class Tile : public Entity{
    TileInfo info;
public:
    explicit Tile(TileInfo& info) : info(info){
        this->posX = info.posx;
        this->posY = info.posy;
        this->rot = info.rotation;
    }
    void draw(Camera& camera) override {
        camera.drawWorldTexture(info.type, posX, posY, rot);
    }
};
#endif //MICROMACHINES_TILE_H
