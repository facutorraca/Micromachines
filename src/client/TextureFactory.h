//
// Created by javier on 24/10/19.
//

#ifndef MICROMACHINES_TEXTUREFACTORY_H
#define MICROMACHINES_TEXTUREFACTORY_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <common/EntityType.h>
#include <iostream>

#define CAR_TEX 4500
#define DMG_CAR_TEX 4501
#define WRECKED_CAR_TEX 45011
#define WHEEL_TEX 4502
#define SPEEDOMETER_TEX 4503
#define SPEEDOBORDER_TEX 4504
#define SPEEDOBAR_TEX 4505
#define LOADING_SCR 4506
#define HEALTH_01 5001
#define HEALTH_02 5002
#define HEALTH_03 5003
#define HEALTH_04 5004
#define HEALTH_05 5005
#define HEALTH_06 5006
#define HEALTH_07 5007
#define HEALTH_08 5008
#define HEALTH_09 5009
#define HEALTH_10 5010
#define HEALTH_11 5011
#define HEALTH_12 5012
#define HEALTH_13 5013
#define HEALTH_14 5014
#define HEALTH_15 5015
#define HEALTH_16 5016
#define WHITE_BLOCK 4507
#define RED_DOT 4508
#define MUD_TEX 4509
#define ROCK_TEX 4510
#define BOOST_TEX 4511



struct Texture{
    SDL_Texture* tex;
    int32_t width;
    int32_t height;
};

class TextureFactory {
    std::map<int32_t, Texture> textures;
    static Texture createTexture(SDL_Renderer* renderer, const char* path){
        SDL_Surface* surface = IMG_Load(path);
        if (surface){
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            int w = surface->w, h = surface->h;
            SDL_FreeSurface(surface);
            return {tex, w, h};
        } else {
            std::cerr << "Error loading texture: " << path << std::endl;
        }
        return {nullptr, 0, 0};
    }
public:
    explicit TextureFactory(SDL_Renderer* renderer){
        if (renderer) {
            textures.emplace(CAR_TEX, createTexture(renderer, "assets/sprites/Cars/Car red striped/Car red striped front.png"));
            textures.emplace(DMG_CAR_TEX, createTexture(renderer, "assets/sprites/Cars/Car red striped/Red striped car crashed.png"));
            textures.emplace(WRECKED_CAR_TEX, createTexture(renderer, "assets/sprites/Cars/Dead car front.png"));
            textures.emplace(WHEEL_TEX, createTexture(renderer, "assets/sprites/wheel_3.png"));
            textures.emplace(SPEEDOMETER_TEX, createTexture(renderer, "assets/sprites/speedometer.png"));
            textures.emplace(SPEEDOBORDER_TEX, createTexture(renderer, "assets/sprites/UI/Oil bar 2.png"));
            textures.emplace(SPEEDOBAR_TEX, createTexture(renderer, "assets/sprites/UI/Oil bar.png"));
            textures.emplace(LOADING_SCR, createTexture(renderer, "assets/sprites/loading_screen.jpg"));
            textures.emplace(WHITE_BLOCK, createTexture(renderer, "assets/sprites/white_block.png"));
            textures.emplace(RED_DOT, createTexture(renderer, "assets/sprites/red_dot.png"));
            textures.emplace(HEALTH_01, createTexture(renderer, "assets/sprites/HealthBar/100.png"));
            textures.emplace(HEALTH_02, createTexture(renderer, "assets/sprites/HealthBar/92.png"));
            textures.emplace(HEALTH_03, createTexture(renderer, "assets/sprites/HealthBar/85.png"));
            textures.emplace(HEALTH_04, createTexture(renderer, "assets/sprites/HealthBar/78.png"));
            textures.emplace(HEALTH_05, createTexture(renderer, "assets/sprites/HealthBar/71.png"));
            textures.emplace(HEALTH_06, createTexture(renderer, "assets/sprites/HealthBar/64.png"));
            textures.emplace(HEALTH_07, createTexture(renderer, "assets/sprites/HealthBar/57.png"));
            textures.emplace(HEALTH_08, createTexture(renderer, "assets/sprites/HealthBar/50.png"));
            textures.emplace(HEALTH_09, createTexture(renderer, "assets/sprites/HealthBar/43.png"));
            textures.emplace(HEALTH_10, createTexture(renderer, "assets/sprites/HealthBar/37.png"));
            textures.emplace(HEALTH_11, createTexture(renderer, "assets/sprites/HealthBar/30.png"));
            textures.emplace(HEALTH_12, createTexture(renderer, "assets/sprites/HealthBar/24.png"));
            textures.emplace(HEALTH_13, createTexture(renderer, "assets/sprites/HealthBar/18.png"));
            textures.emplace(HEALTH_14, createTexture(renderer, "assets/sprites/HealthBar/12.png"));
            textures.emplace(HEALTH_15, createTexture(renderer, "assets/sprites/HealthBar/6.png"));
            textures.emplace(HEALTH_16, createTexture(renderer, "assets/sprites/HealthBar/0.png"));

            textures.emplace(TYPE_BOOST, createTexture(renderer, "assets/sprites/modifiers/boost.png"));
            textures.emplace(TYPE_FIX, createTexture(renderer, "assets/sprites/modifiers/fix.png"));
            textures.emplace(TYPE_MUD, createTexture(renderer, "assets/sprites/modifiers/mud.png"));
            textures.emplace(TYPE_OIL, createTexture(renderer, "assets/sprites/modifiers/oil.png"));
            textures.emplace(TYPE_ROCK, createTexture(renderer, "assets/sprites/modifiers/rock.png"));

            /*-------------------------------SCREEN EFFECT TEXTURES--------------------------------*/

            textures.emplace(MUD_TEX, createTexture(renderer, "assets/sprites/screen_effects/mud.png"));
            textures.emplace(ROCK_TEX, createTexture(renderer, "assets/sprites/screen_effects/rock.png"));
            textures.emplace(BOOST_TEX, createTexture(renderer, "assets/sprites/screen_effects/boost.png"));

            /*-------------------------------ASPHALT_ROADS-----------------------------------------*/
            int tile_width = TILE_TERRAIN_SIZE*METER_TO_PIXEL+5;
            textures.emplace(3, createTexture(renderer,"assets/sprites/Track/AR_3a.png")); 
            textures.emplace(4, createTexture(renderer,"assets/sprites/Track/AR_4a.png")); 
            textures.emplace(5, createTexture(renderer,"assets/sprites/Track/AR_5a.png")); 
            textures.emplace(8, createTexture(renderer,"assets/sprites/Track/AR_8a.png")); 
            textures.emplace(9, createTexture(renderer,"assets/sprites/Track/AR_9a.png")); 
            textures.emplace(10, createTexture(renderer,"assets/sprites/Track/AR_10a.png")); 
            textures.emplace(13, createTexture(renderer,"assets/sprites/Track/AR_13a.png")); 
            textures.emplace(14, createTexture(renderer,"assets/sprites/Track/AR_14a.png")); 
            textures.emplace(15, createTexture(renderer,"assets/sprites/Track/AR_15a.png")); 
            textures.emplace(16, createTexture(renderer,"assets/sprites/Track/AR_16a.png")); 
            textures.emplace(17, createTexture(renderer,"assets/sprites/Track/AR_17a.png")); 
            textures.emplace(18, createTexture(renderer,"assets/sprites/Track/AR_18a.png")); 
            textures.emplace(19, createTexture(renderer,"assets/sprites/Track/AR_19a.png")); 
            textures.emplace(21, createTexture(renderer,"assets/sprites/Track/AR_21a.png")); 
            textures.emplace(22, createTexture(renderer,"assets/sprites/Track/AR_22a.png")); 
            textures.emplace(25, createTexture(renderer,"assets/sprites/Track/AR_25a.png")); 
            textures.emplace(27, createTexture(renderer,"assets/sprites/Track/AR_27a.png")); 

            /*-----------------------------------DIRT_ROADS----------------------------------------*/
            textures.emplace(28, createTexture(renderer,"assets/sprites/Track/D_1a.png")); 
            textures.emplace(31, createTexture(renderer,"assets/sprites/Track/DR_3a.png")); 
            textures.emplace(32, createTexture(renderer,"assets/sprites/Track/DR_4a.png")); 
            textures.emplace(33, createTexture(renderer,"assets/sprites/Track/DR_5a.png")); 
            textures.emplace(36, createTexture(renderer,"assets/sprites/Track/DR_8a.png")); 
            textures.emplace(41, createTexture(renderer,"assets/sprites/Track/DR_13a.png")); 
            textures.emplace(42, createTexture(renderer,"assets/sprites/Track/DR_14a.png")); 
            textures.emplace(44, createTexture(renderer,"assets/sprites/Track/DR_16a.png")); 
            textures.emplace(55, createTexture(renderer,"assets/sprites/Track/DR_27a.png")); 

            /*-----------------------------------------GRASS---------------------------------------*/
            textures.emplace(56, createTexture(renderer,"assets/sprites/Track/LG_1a.png"));
            textures.emplace(57, createTexture(renderer,"assets/sprites/Track/LG_2a.png"));
            textures.emplace(58, createTexture(renderer,"assets/sprites/Track/LG_3a.png"));
            textures.emplace(59, createTexture(renderer,"assets/sprites/Track/LG_4a.png")); 

            /*-----------------------------------SAND_ROADS----------------------------------------*/
            textures.emplace(63, createTexture(renderer,"assets/sprites/Track/LS_4a.png")); 
            textures.emplace(66, createTexture(renderer,"assets/sprites/Track/SR_3a.png")); 
            textures.emplace(67, createTexture(renderer,"assets/sprites/Track/SR_4a.png")); 
            textures.emplace(78, createTexture(renderer,"assets/sprites/Track/SR_16a.png")); 
            textures.emplace(89, createTexture(renderer,"assets/sprites/Track/SR_27a.png")); 

            /*-----------------------------------TRACK_OBJECTS-------------------------------------*/
            textures.emplace(102, createTexture(renderer,"assets/sprites/TrackObjects/TD_1.png")); 
            textures.emplace(103, createTexture(renderer,"assets/sprites/TrackObjects/TD_2.png")); 
            textures.emplace(104, createTexture(renderer,"assets/sprites/TrackObjects/TD_3.png")); 
            textures.emplace(105, createTexture(renderer,"assets/sprites/TrackObjects/TD_4.png")); 
            textures.emplace(106, createTexture(renderer,"assets/sprites/TrackObjects/TD_5.png")); 
            textures.emplace(107, createTexture(renderer,"assets/sprites/TrackObjects/TD_6.png")); 
            textures.emplace(108, createTexture(renderer,"assets/sprites/TrackObjects/TD_7.png")); 
            textures.emplace(109, createTexture(renderer,"assets/sprites/TrackObjects/TD_8.png")); 
            textures.emplace(110, createTexture(renderer,"assets/sprites/TrackObjects/TD_9.png")); 
            textures.emplace(111, createTexture(renderer,"assets/sprites/TrackObjects/TD_10.png")); 

            textures.emplace(112, createTexture(renderer,"assets/sprites/TrackObjects/TS_1.png")); 

            textures.emplace(114, createTexture(renderer,"assets/sprites/TrackObjects/B_1a.png")); 
            textures.emplace(115, createTexture(renderer,"assets/sprites/TrackObjects/B_2a.png")); 
            textures.emplace(116, createTexture(renderer,"assets/sprites/TrackObjects/W_1a.png")); 
            textures.emplace(117, createTexture(renderer,"assets/sprites/TrackObjects/W_2a.png")); 
            textures.emplace(118, createTexture(renderer,"assets/sprites/TrackObjects/W_4a.png")); 
            textures.emplace(119, createTexture(renderer,"assets/sprites/TrackObjects/W_5a.png")); 
            textures.emplace(120, createTexture(renderer,"assets/sprites/TrackObjects/W_6a.png")); 
            textures.emplace(121, createTexture(renderer,"assets/sprites/TrackObjects/W_3a.png")); 

            textures.emplace(122, createTexture(renderer,"assets/sprites/TrackObjects/S_1.png")); 
            textures.emplace(123, createTexture(renderer,"assets/sprites/TrackObjects/S_2.png")); 

            textures.emplace(124, createTexture(renderer,"assets/sprites/TrackObjects/TB_1a.png"));
            textures.emplace(125, createTexture(renderer,"assets/sprites/TrackObjects/trophy_s.png"));
            textures.emplace(126, createTexture(renderer,"assets/sprites/TrackObjects/trophy_g.png"));
            textures.emplace(127, createTexture(renderer,"assets/sprites/TrackObjects/trophy_b.png"));


            textures.emplace(128, createTexture(renderer,"assets/sprites/TrackObjects/PF_1a.png"));
            textures.emplace(129, createTexture(renderer,"assets/sprites/TrackObjects/PF_2a.png"));
            textures.emplace(130, createTexture(renderer,"assets/sprites/TrackObjects/PF_3a.png"));
            textures.emplace(131, createTexture(renderer,"assets/sprites/TrackObjects/PF_4a.png"));
            textures.emplace(132, createTexture(renderer,"assets/sprites/TrackObjects/PF_5a.png"));

            textures.emplace(133, createTexture(renderer,"assets/sprites/TrackObjects/PS_1a.png"));
            textures.emplace(134, createTexture(renderer,"assets/sprites/TrackObjects/PS_2a.png"));
            textures.emplace(135, createTexture(renderer,"assets/sprites/TrackObjects/PS_3a.png"));
            textures.emplace(136, createTexture(renderer,"assets/sprites/TrackObjects/PS_4a.png"));
            textures.emplace(137, createTexture(renderer,"assets/sprites/TrackObjects/PS_5a.png"));
            textures.emplace(138, createTexture(renderer,"assets/sprites/TrackObjects/PT_1a.png"));
            textures.emplace(139, createTexture(renderer,"assets/sprites/TrackObjects/PT_2a.png"));
            textures.emplace(140, createTexture(renderer,"assets/sprites/TrackObjects/PT_3a.png"));
            textures.emplace(141, createTexture(renderer,"assets/sprites/TrackObjects/PT_4a.png"));
            textures.emplace(142, createTexture(renderer,"assets/sprites/TrackObjects/PT_5a.png"));

            textures.emplace(143, createTexture(renderer,"assets/sprites/TrackObjects/PF_1b.png"));
            textures.emplace(144, createTexture(renderer,"assets/sprites/TrackObjects/PS_1b.png"));
            textures.emplace(145, createTexture(renderer,"assets/sprites/TrackObjects/PT_1b.png"));
        }
    }

    TextureFactory& operator=(TextureFactory&& other) noexcept{
        this->textures = std::move(other.textures);
        return *this;
    }

    Texture getTexture(int32_t id){
        try {
            return textures.at(id);
        } catch (std::out_of_range& e) {
            std::cerr << "TextureFactory: Texture not found: " << std::to_string(id) << std::endl;
            return {nullptr, 0, 0};
        }
    }

    ~TextureFactory(){
        for (auto& tex : textures)
            SDL_DestroyTexture(tex.second.tex);
    }
};

#endif //MICROMACHINES_TEXTUREFACTORY_H
