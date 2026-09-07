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

void Player::_physics_process(double delta) {
}