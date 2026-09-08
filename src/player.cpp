#include "player.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Player::Player() {
    
}

Player::~Player() {
}

void Player::_bind_methods() {
}

void Player::_ready() {
    UtilityFunctions::print("Player C++ loaded!");
}



//HÀM TÍNH SÁT THƯƠNG
void Player::take_damage(int damage) {
    health -= damage;

    UtilityFunctions::print("Player took damage: ", damage);
    UtilityFunctions::print("HP left: ", health);

    if (health <= 0) {
        health = 0;
        die();
    }
}

void Player::die() {
    UtilityFunctions::print("Player died!");
}


void Player::_physics_process(double delta) {


    //MOVEMENT CODE
    Input *input = Input::get_singleton();

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