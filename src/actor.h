#pragma once

#include <godot_cpp/classes/character_body2d.hpp>

namespace godot {

class Actor : public CharacterBody2D {
    GDCLASS(Actor, CharacterBody2D)

protected:
    int health = 0;
    int max_health = 0;
    float move_speed = 0.0f;
    float move_speed_negation = 0.0f;


    // ==========================================
    // COMBAT STATE
    // ==========================================

    enum class CombatState {
        IDLE,
        ATTACKING,
        PARRY_START_UP,
        PARRYING,
        BLOCKING,
        STUNNED
    };

    CombatState combat_state = CombatState::IDLE;


    static void _bind_methods();


public:
    Actor();
    ~Actor();

    void take_damage(int damage);
    void die();

    void stun();
    void recover_from_stun();
};

}