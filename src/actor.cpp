#include "actor.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;


Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::_bind_methods() {

    ClassDB::bind_method(D_METHOD("get_health"), &Actor::get_health);
    ClassDB::bind_method(D_METHOD("get_max_health"), &Actor::get_max_health);
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

void Actor::die(bool play_sound) {
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



int Actor::get_health() const {
    return health;
}

int Actor::get_max_health() const {
    return max_health;
}