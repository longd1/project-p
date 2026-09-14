#include "player.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;


// ==================================================
// CONSTRUCTOR / DESTRUCTOR
// ==================================================

Player::Player() {
    max_health = 100;
    health = max_health;
    move_speed = 150.0f;
}

Player::~Player() {
}

void Player::_bind_methods() {
}


// ==================================================
// ATTACK
// ==================================================

void Player::attack() {

    if (combat_state != CombatState::IDLE) {
        return;
    }

    combat_state = CombatState::ATTACKING;
    attack_timer = attack_duration;

    UtilityFunctions::print("John attacks!");
}


// ==================================================
// PARRY
// ==================================================

void Player::parry_start_up() {


    if (combat_state != CombatState::IDLE) {
        return;
    }

    combat_state = CombatState::PARRY_START_UP;

    parry_timer = parry_duration;

}

void Player::parry() {
    combat_state = CombatState::PARRYING;

    UtilityFunctions::print("Parry window started!");
}


// ==================================================
// BLOCK
// ==================================================

void Player::block() {


    UtilityFunctions::print("Attack blocked!");
}


// ==================================================
// READY
// ==================================================

void Player::_ready() {

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    UtilityFunctions::print("Player C++ loaded!");

    InputMap::get_singleton()->load_from_project_settings();
}


// ==================================================
// PHYSICS PROCESS
// ==================================================

void Player::_physics_process(double delta) {

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Input *input = Input::get_singleton();


    // ==================================================
    // ATTACK LOGIC
    // ==================================================

    if (combat_state == CombatState::ATTACKING) {

        attack_timer -= delta;

        if (attack_timer <= 0.0f) {
            attack_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }
    }


    // ==================================================
    // PARRY LOGIC
    // ==================================================

    else if (combat_state == CombatState::PARRY_START_UP) {

        if (input->is_action_just_released("parry")) {
            parry_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }

        else {
            parry_timer -= delta;

            if (parry_timer <= parry_duration - pre_parry_duration) {
                parry();
            }
        }
    }


    else if (combat_state == CombatState::PARRYING) {

        parry_timer -= delta;

        if (parry_timer <= 0.0f) {

            parry_timer = 0.0f;

            if (input->is_action_pressed("parry")) {
                combat_state = CombatState::BLOCKING;
            }
            else {
                combat_state = CombatState::IDLE;
            }
        }
    }


    // ==================================================
    // BLOCK LOGIC
    // ==================================================

    else if (combat_state == CombatState::BLOCKING) {

        if (input->is_action_just_released("parry")) {
            combat_state = CombatState::IDLE;
        }
    }


    // ==================================================
    // COMBAT INPUT
    // ==================================================

    if (input->is_action_just_pressed("attack")) {
        attack();
    }

    if (input->is_action_just_pressed("parry")) {
        parry_start_up();
    }


    // ==================================================
    // MOVEMENT
    // ==================================================

    Vector2 direction(0, 0);

    if (input->is_action_pressed("move_left")) {
        direction.x -= 1;
    }

    if (input->is_action_pressed("move_right")) {
        direction.x += 1;
    }

    if (input->is_action_pressed("move_up")) {
        direction.y -= 1;
    }

    if (input->is_action_pressed("move_down")) {
        direction.y += 1;
    }

    if (direction.length() > 0) {
        direction = direction.normalized();
    }

    set_velocity(direction * move_speed);
    move_and_slide();
}