#include "player.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

Player::Player() {
    max_health = 100;
    health = max_health;
    move_speed = 200.0f;
}

Player::~Player() {
}
void Player::_bind_methods() {
}

void Player::_ready() {
    UtilityFunctions::print("Player C++ loaded!");
    InputMap::get_singleton()->load_from_project_settings();
}

void Player::_physics_process(double delta) {

    // MOVEMENT CODE
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    godot::Input *input = godot::Input::get_singleton();

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