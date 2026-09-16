#pragma once

#include "actor.h"

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:
    int max_mana = 100;
    int mana = max_mana;


    // ==========================================
    // ATTACK
    // ==========================================

    float attack_duration = 0.25f;
    float attack_timer = 0.0f;


    // ==========================================
    // PARRY
    // ==========================================

    float parry_duration = 0.25f;
    float parry_timer = 0.0f;
    float pre_parry_duration = 0.05f;


protected:
    static void _bind_methods();


public:
    Player();
    ~Player();

    void _ready() override;
    void _physics_process(double delta) override;

    void attack();

    void parry();
    void parry_start_up();

    void block();
};

}