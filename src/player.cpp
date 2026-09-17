#include "player.h"

#include <godot_cpp/core/class_db.hpp>
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
    move_speed_negation = 1.0f;
}

Player::~Player() {
}


// ==================================================
// BIND METHODS
// ==================================================

void Player::_bind_methods() {

    ClassDB::bind_method(
        D_METHOD("set_current_weapon", "weapon"),
        &Player::set_current_weapon
    );

    ClassDB::bind_method(
        D_METHOD("get_current_weapon"),
        &Player::get_current_weapon
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::OBJECT,
            "current_weapon",
            PROPERTY_HINT_RESOURCE_TYPE,
            "WeaponData"
        ),
        "set_current_weapon",
        "get_current_weapon"
    );
}


// ==================================================
// WEAPON
// ==================================================

void Player::set_current_weapon(const Ref<WeaponData> &weapon) {

    current_weapon = weapon;

    if (current_weapon.is_valid()) {

        UtilityFunctions::print(
            "Equipped weapon: ",
            current_weapon->get_weapon_name()
        );
    }
}


Ref<WeaponData> Player::get_current_weapon() const {

    return current_weapon;
}


int Player::get_attack_damage() const {

    if (current_weapon.is_valid()) {

        return base_damage + current_weapon->get_damage();
    }

    return base_damage;
}


float Player::get_block_damage_negation() const {

    if (current_weapon.is_valid()) {

        return current_weapon->get_block_damage_negation();
    }

    return 0.0f;
}


// ==================================================
// ATTACK
// ==================================================

void Player::attack() {

    if (combat_state != CombatState::IDLE) {
        return;
    }

    if (!current_weapon.is_valid()) {
        return;
    }

    combat_state = CombatState::ATTACKING;

    attack_timer = current_weapon->get_attack_duration();

    // Đảm bảo attack luôn chạy xuôi.
    animated_sprite->set_speed_scale(1.0f);

    animated_sprite->play("attack");
}


// ==================================================
// PARRY
// ==================================================

void Player::parry_start_up() {

    if (combat_state != CombatState::IDLE) {
        return;
    }

    if (!current_weapon.is_valid()) {
        return;
    }

    combat_state = CombatState::PARRY_START_UP;

    parry_timer = current_weapon->get_parry_duration();

    animated_sprite->set_speed_scale(1.0f);

    animated_sprite->play("block");
}


void Player::parry() {

    combat_state = CombatState::PARRYING;

    UtilityFunctions::print("Parry window started!");
}


// ==================================================
// BLOCK
// ==================================================

void Player::block() {

    UtilityFunctions::print(
        "Attack blocked! Negation: ",
        get_block_damage_negation()
    );
}


// ==================================================
// READY
// ==================================================

void Player::_ready() {

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    InputMap::get_singleton()->load_from_project_settings();

    animated_sprite = get_node<AnimatedSprite2D>(
        NodePath("AnimatedSprite2D")
    );

    animated_sprite->play("idle");
}


// ==================================================
// PHYSICS PROCESS
// ==================================================

void Player::_physics_process(double delta) {

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Input *input = Input::get_singleton();

    move_speed_negation = 1.0f;


    // ==================================================
    // STUNNED
    // ==================================================

    if (combat_state == CombatState::STUNNED) {

        set_velocity(Vector2(0, 0));

        move_and_slide();

        return;
    }


    // ==================================================
    // ATTACK LOGIC
    // ==================================================

    if (combat_state == CombatState::ATTACKING) {

        move_speed_negation = 0.45f;

        attack_timer -= delta;

        if (attack_timer <= 0.0f) {

            attack_timer = 0.0f;

            combat_state = CombatState::IDLE;
        }
    }


    // ==================================================
    // PARRY STARTUP LOGIC
    // ==================================================

    else if (combat_state == CombatState::PARRY_START_UP) {

        move_speed_negation = 0.45f;

        if (input->is_action_just_released("parry")) {

            parry_timer = 0.0f;

            combat_state = CombatState::IDLE;


            if (animated_sprite->is_playing()) {

                animated_sprite->set_speed_scale(-1.0f);
            }

            else {

                animated_sprite->set_speed_scale(1.0f);

                animated_sprite->play_backwards("block");
            }
        }

        else {

            parry_timer -= delta;

            if (
                parry_timer <=
                current_weapon->get_parry_duration()
                - current_weapon->get_pre_parry_duration()
            ) {
                parry();
            }
        }
    }


    // ==================================================
    // PARRY WINDOW LOGIC
    // ==================================================

    else if (combat_state == CombatState::PARRYING) {

        move_speed_negation = 0.45f;

        parry_timer -= delta;

        if (parry_timer <= 0.0f) {

            parry_timer = 0.0f;

            if (input->is_action_pressed("parry")) {

                combat_state = CombatState::BLOCKING;
            }

            else {

                combat_state = CombatState::IDLE;


                if (animated_sprite->is_playing()) {

                    animated_sprite->set_speed_scale(-1.0f);
                }

                else {

                    animated_sprite->set_speed_scale(1.0f);

                    animated_sprite->play_backwards("block");
                }
            }
        }
    }


    // ==================================================
    // BLOCK LOGIC
    // ==================================================

    else if (combat_state == CombatState::BLOCKING) {

        move_speed_negation = 0.45f;

        if (input->is_action_just_released("parry")) {

            combat_state = CombatState::IDLE;


            if (animated_sprite->is_playing()) {

                animated_sprite->set_speed_scale(-1.0f);
            }

            else {

                animated_sprite->set_speed_scale(1.0f);

                animated_sprite->play_backwards("block");
            }
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


    // ==================================================
    // FLIP SPRITE
    // ==================================================

    if (direction.x < 0) {

        animated_sprite->set_flip_h(true);
    }

    else if (direction.x > 0) {

        animated_sprite->set_flip_h(false);
    }


    if (direction.length() > 0) {

        direction = direction.normalized();
    }


    // ==================================================
    // MOVEMENT ANIMATION
    // ==================================================

    if (combat_state == CombatState::IDLE) {

        bool animation_is_reversing =
            animated_sprite->is_playing()
            && animated_sprite->get_playing_speed() < 0.0f;


        if (!animation_is_reversing) {

            animated_sprite->set_speed_scale(1.0f);

            if (direction.length() > 0) {

                animated_sprite->play("run");
            }

            else {

                animated_sprite->play("idle");
            }
        }
    }


    // ==================================================
    // APPLY MOVEMENT
    // ==================================================

    set_velocity(
        direction * move_speed * move_speed_negation
    );

    move_and_slide();
}