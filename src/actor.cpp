#include "actor.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::_bind_methods() {
}

void Actor::take_damage(int damage) {
    health -= damage;

    UtilityFunctions::print("Damage taken: ", damage);
    UtilityFunctions::print("Health remaining: ", health);

    if (health <= 0) {
        health = 0;
        die();
    }
}

void Actor::die() {
    UtilityFunctions::print("Actor died!");
}