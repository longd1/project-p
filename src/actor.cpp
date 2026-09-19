#include "actor.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;


Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::_bind_methods() {
}


// ==========================================
// DAMAGE
// ==========================================

void Actor::take_damage(int damage, float stun_scale, float knockback, Vector2 attacker_position) {
    health -= damage;

    if (health < 0) {
        health = 0;
    }
}


// ==========================================
// DEATH
// ==========================================

void Actor::die() {
    //UtilityFunctions::print("Actor died!");
}


// ==========================================
// STUN
// ==========================================

void Actor::stun() {

    combat_state = CombatState::STUNNED;

    UtilityFunctions::print("Actor stunned!");
}


// ==========================================
// RECOVER FROM STUN
// ==========================================

void Actor::recover_from_stun() {

    if (combat_state == CombatState::STUNNED) {
        combat_state = CombatState::IDLE;
    }
}