#pragma once

#include "actor.h"

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:
    int max_mana = 100;
    int mana = max_mana;

    enum class CombatState {
    IDLE,
    ATTACKING,
    PARRYING,
    BLOCKING,
    PARRY_START_UP,
};


    CombatState combat_state = CombatState::IDLE;

    float attack_duration = 0.25f;
    float attack_timer = 0.0f;

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