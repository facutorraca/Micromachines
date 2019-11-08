#ifndef MICROMACHINES_WHEELSTATE_H
#define MICROMACHINES_WHEELSTATE_H

#include <mutex>
#include "Box2D/Box2D.h"
#include <model/Modifiers/Effect.h>
#include <model/Modifiers/NoEffect.h>

class WheelState {

    std::shared_ptr<Effect> effect;

    float max_driver_force;
    float max_forward_speed;
    float max_backward_speed;
    float max_lateral_impulse;

    std::mutex mtx;

    public:
        WheelState(float max_forward_speed, float max_backward_speed, float max_driver_force, float max_lateral_impulse):
            effect(new NoEffect())
        {
            this->max_lateral_impulse = max_lateral_impulse; /*Capacity of changing direction without skidding*/
            this->max_backward_speed = max_backward_speed;
            this->max_forward_speed = max_forward_speed;
            this->max_driver_force = max_driver_force; /*Capacity of changing speed*/
        }

        void apply_effect(std::unique_ptr<Effect> new_effect) {
            std::unique_lock<std::mutex> lock(this->mtx);
            this->effect = std::move(new_effect);
        }

        void update() {
            std::unique_lock<std::mutex> lock(this->mtx);
            this->effect->update();
            if (this->effect->is_over()) {
                this->effect = this->effect->get_next_effect();
            }
        }

        float get_max_lateral_impulse() {
            return this->effect->apply_to_lateral_impulse(this->max_lateral_impulse);
        }

        float get_max_backward_speed() {
            return this->effect->apply_to_backward_speed(this->max_backward_speed);
        }

        float get_max_forward_speed() {
            return this->effect->apply_to_forward_speed(this->max_forward_speed);
        }

        virtual float get_max_driver_force() {
            return this->effect->apply_to_driver_force(this->max_driver_force);
        }
};



#endif //MICROMACHINES_WHEELSTATE_H
