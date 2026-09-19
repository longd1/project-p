#include "player.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>

using namespace godot;


Player::Player() {
    max_health = 100;
    health = max_health;

    move_speed = 150.0f;
    move_speed_negation = 1.0f;
}


Player::~Player() {
}


void Player::_bind_methods() {

    ADD_SIGNAL(MethodInfo("died"));

    ClassDB::bind_method(D_METHOD("set_current_weapon", "weapon"), &Player::set_current_weapon);
    ClassDB::bind_method(D_METHOD("get_current_weapon"), &Player::get_current_weapon);
    ClassDB::bind_method(D_METHOD("die", "play_sound"), &Player::die, DEFVAL(true));

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "current_weapon", PROPERTY_HINT_RESOURCE_TYPE, "WeaponData"),
        "set_current_weapon",
        "get_current_weapon"
    );
}


void Player::set_current_weapon(const Ref<WeaponData> &weapon) {
    current_weapon = weapon;

    if (current_weapon.is_valid()) {
        UtilityFunctions::print("Equipped weapon: ", current_weapon->get_weapon_name());
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


void Player::attack() {
    if (combat_state != CombatState::IDLE) {
        return;
    }

    if (!current_weapon.is_valid()) {
        return;
    }

    combat_state = CombatState::ATTACKING;
    attack_timer = current_weapon->get_attack_duration();
    attack_has_hit = false;

    Ref<SpriteFrames> sprite_frames = animated_sprite->get_sprite_frames();

    int frame_count = sprite_frames->get_frame_count(StringName("attack"));
    double animation_fps = sprite_frames->get_animation_speed(StringName("attack"));
    double animation_duration = 0.0;

    for (int i = 0; i < frame_count; i++) {
        animation_duration += sprite_frames->get_frame_duration(StringName("attack"), i) / animation_fps;
    }

    double animation_speed = animation_duration / current_weapon->get_attack_duration();

    animated_sprite->stop();
    animated_sprite->set_speed_scale(animation_speed);
    animated_sprite->play("attack");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    hitbox->set_monitoring(true);
}


void Player::parry_start_up() {
    if (combat_state != CombatState::IDLE) {
        return;
    }

    if (!current_weapon.is_valid()) {
        return;
    }

    combat_state = CombatState::PARRY_START_UP;
    parry_timer = current_weapon->get_parry_duration();

    Ref<SpriteFrames> sprite_frames = animated_sprite->get_sprite_frames();

    int frame_count = sprite_frames->get_frame_count(StringName("block"));
    double animation_fps = sprite_frames->get_animation_speed(StringName("block"));
    double animation_duration = 0.0;

    for (int i = 0; i < frame_count; i++) {
        animation_duration += sprite_frames->get_frame_duration(StringName("block"), i) / animation_fps;
    }

    double animation_speed = animation_duration / current_weapon->get_parry_duration();

    animated_sprite->stop();
    animated_sprite->set_speed_scale(animation_speed);
    animated_sprite->play("block");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}


void Player::parry() {
    combat_state = CombatState::PARRYING;
}


void Player::block() {
}


void Player::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    InputMap::get_singleton()->load_from_project_settings();

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));
    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    death_sound = get_node<AudioStreamPlayer2D>(NodePath("sound/DeathSound"));

    hitbox->set_monitoring(false);

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}

void Player::die(bool play_sound) {
    hitbox->set_monitoring(false);

    set_velocity(Vector2(0, 0));
    set_physics_process(false);

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("die");
    animated_sprite->set_frame_and_progress(0, 0.0f);



    if (play_sound && death_sound != nullptr) {
        death_sound->play();
    }

    emit_signal("died");
}









void Player::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Input *input = Input::get_singleton();

    move_speed_negation = 1.0f;


    if (combat_state == CombatState::STUNNED) {
        hitbox->set_monitoring(false);

        set_velocity(Vector2(0, 0));
        move_and_slide();

        return;
    }


    if (combat_state == CombatState::ATTACKING) {
        move_speed_negation = 0.45f;

        attack_timer -= delta;

        int attack_frame = animated_sprite->get_frame();

        if (attack_frame <= 2) {
            hitbox->set_monitoring(true);

            TypedArray<Area2D> overlapping_areas = hitbox->get_overlapping_areas();

            for (int i = 0; i < overlapping_areas.size(); i++) {
                Area2D *area = Object::cast_to<Area2D>(overlapping_areas[i]);

                if (area == nullptr || attack_has_hit) {
                    continue;
                }

                Node *target = area;

                while (target != nullptr) {
                    Actor *actor = Object::cast_to<Actor>(target);

                    if (actor != nullptr && actor != this) {
                        actor->take_damage(get_attack_damage(), current_weapon->get_stun_scale(), current_weapon->get_knockback(), get_global_position());

                        attack_has_hit = true;
                        break;
                    }

                    target = target->get_parent();
                }
            }
        }
        else {
            hitbox->set_monitoring(false);
        }

        if (attack_timer <= 0.0f) {
            attack_timer = 0.0f;
            hitbox->set_monitoring(false);

            combat_state = CombatState::IDLE;
        }
    }


    else if (combat_state == CombatState::PARRY_START_UP) {
        move_speed_negation = 0.45f;

        if (input->is_action_just_released("parry")) {
            parry_timer = 0.0f;
            combat_state = CombatState::IDLE;

            double block_speed = animated_sprite->get_speed_scale();

            if (block_speed < 0.0) {
                block_speed = -block_speed;
            }

            if (animated_sprite->is_playing()) {
                animated_sprite->set_speed_scale(-block_speed);
            }
            else {
                animated_sprite->set_speed_scale(block_speed);
                animated_sprite->play_backwards("block");
            }
        }
        else {
            parry_timer -= delta;

            if (parry_timer <= current_weapon->get_parry_duration() - current_weapon->get_pre_parry_duration()) {
                parry();
            }
        }
    }


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

                double block_speed = animated_sprite->get_speed_scale();

                if (block_speed < 0.0) {
                    block_speed = -block_speed;
                }

                if (animated_sprite->is_playing()) {
                    animated_sprite->set_speed_scale(-block_speed);
                }
                else {
                    animated_sprite->set_speed_scale(block_speed);
                    animated_sprite->play_backwards("block");
                }
            }
        }
    }


    else if (combat_state == CombatState::BLOCKING) {
        move_speed_negation = 0.45f;

        if (input->is_action_just_released("parry")) {
            combat_state = CombatState::IDLE;

            double block_speed = animated_sprite->get_speed_scale();

            if (block_speed < 0.0) {
                block_speed = -block_speed;
            }

            if (animated_sprite->is_playing()) {
                animated_sprite->set_speed_scale(-block_speed);
            }
            else {
                animated_sprite->set_speed_scale(block_speed);
                animated_sprite->play_backwards("block");
            }
        }
    }


    if (input->is_action_just_pressed("attack")) {
        attack();
    }

    if (input->is_action_just_pressed("parry")) {
        parry_start_up();
    }


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


    if (direction.x < 0) {
        animated_sprite->set_flip_h(true);
    }
    else if (direction.x > 0) {
        animated_sprite->set_flip_h(false);
    }


    if (direction.length() > 0) {
        direction = direction.normalized();
    }


    if (combat_state == CombatState::IDLE) {
        bool animation_is_reversing = animated_sprite->is_playing() && animated_sprite->get_playing_speed() < 0.0f;

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


    set_velocity(direction * move_speed * move_speed_negation);

    move_and_slide();
}