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
    ADD_SIGNAL(MethodInfo("normal_attack_started", PropertyInfo(Variant::BOOL, "facing_left")));

    ClassDB::bind_method(D_METHOD("set_current_weapon", "weapon"), &Player::set_current_weapon);
    ClassDB::bind_method(D_METHOD("get_current_weapon"), &Player::get_current_weapon);
    ClassDB::bind_method(D_METHOD("die", "play_sound"), &Player::die, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("apply_external_knockback", "force"), &Player::apply_external_knockback);
    ClassDB::bind_method(D_METHOD("refresh_progression"), &Player::refresh_progression);
    ClassDB::bind_method(D_METHOD("get_current_heal_flasks"), &Player::get_current_heal_flasks);

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

void Player::refresh_progression() {
    if (global == nullptr) return;

    int bonus_max_health = static_cast<int>(global->get("bonus_max_health"));

    int old_max_health = max_health;
    max_health = 100 + bonus_max_health;

    health += max_health - old_max_health;

    if (health > max_health) {
        health = max_health;
    }
}


Ref<WeaponData> Player::get_current_weapon() const {
    return current_weapon;
}

int Player::get_current_heal_flasks() const {
    return current_heal_flasks;
}

int Player::get_attack_damage() const {
    int damage = base_damage;

    if (current_weapon.is_valid()) {
        damage += current_weapon->get_damage();
    }

    float damage_multiplier = 1.0f;

    if (global != nullptr) {
        damage_multiplier = static_cast<float>(global->get("damage_multiplier"));
    }

    return static_cast<int>(damage * damage_multiplier + 0.5f);
}


float Player::get_block_damage_negation() const {
    if (current_weapon.is_valid()) {
        return current_weapon->get_block_damage_negation();
    }

    return 0.0f;
}


void Player::attack() {
    if (health <= 0) return;
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

    emit_signal("normal_attack_started", animated_sprite->is_flipped_h());
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



void Player::heal() {
    if (current_heal_flasks <= 0) {
        return;
    }


    if (global == nullptr) return;

    int heal_amount = static_cast<int>(global->get("heal_amount"));

    health += heal_amount;

    if (health > max_health) {
        health = max_health;
    }

    current_heal_flasks--;

    UtilityFunctions::print("John healed ", heal_amount, " HP | HP: ", health, "/", max_health, " | Flasks: ", current_heal_flasks);
}



void Player::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    InputMap::get_singleton()->load_from_project_settings();

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));

    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));

    death_sound = get_node<AudioStreamPlayer2D>(NodePath("sound/DeathSound"));
    effect = get_node<AnimatedSprite2D>(NodePath("effect"));

    global = get_node<Node>(NodePath("/root/Global"));

    int bonus_max_health = static_cast<int>(global->get("bonus_max_health"));
    max_health = 100 + bonus_max_health;
    health = max_health;

    current_heal_flasks = static_cast<int>(global->get("max_heal_flasks"));

    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hitbox_default_position = hitbox->get_position();


    parry_sound = get_node<AudioStreamPlayer2D>(NodePath("sound/ParrySound"));
    block_sound = get_node<AudioStreamPlayer2D>(NodePath("sound/BlockSound"));
    damage_sound = get_node<AudioStreamPlayer2D>(NodePath("sound/DamageSound"));

    effect->hide();



    hitbox->set_monitoring(false);

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");

    
}

void Player::die(bool play_sound) {
    hitbox->set_deferred("monitoring", false);
    hurtbox->set_deferred("monitorable", false);

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

void Player::apply_external_knockback(const Vector2 &force) {
    knockback_velocity = force;
}

void Player::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0) return;

    Vector2 knockback_direction = Vector2(0, 0);

    if (attacker != nullptr) {
        knockback_direction = (get_global_position() - attacker->get_global_position()).normalized();
    }



    if (combat_state == CombatState::PARRYING) {
        float parry_knockback = damage * knockback * 0.45f;
        knockback_velocity = knockback_direction * parry_knockback;

        parry_sound->play();

        bool facing_left = animated_sprite->is_flipped_h();

        effect->set_flip_h(facing_left);

        Vector2 effect_position = effect->get_position();
        float effect_offset_x = Math::abs(effect_position.x);
        effect_position.x = facing_left ? -effect_offset_x : effect_offset_x;
        effect->set_position(effect_position);

        effect->show();
        effect->stop();
        effect->set_speed_scale(1.0f);
        effect->play("parry");
        effect->set_frame_and_progress(0, 0.0f);


        return;
    }



    int final_damage = damage;

    if (combat_state == CombatState::BLOCKING) {
        final_damage = static_cast<int>(damage * (1.0f - get_block_damage_negation()));

        block_sound->play();


        bool facing_left = animated_sprite->is_flipped_h();

        effect->set_flip_h(facing_left);

        Vector2 effect_position = effect->get_position();
        float effect_offset_x = Math::abs(effect_position.x);
        effect_position.x = facing_left ? -effect_offset_x : effect_offset_x;
        effect->set_position(effect_position);

        effect->show();
        effect->stop();
        effect->set_speed_scale(1.0f);
        effect->play("block");
        effect->set_frame_and_progress(0, 0.0f);


    }



    health -= final_damage;

    if (final_damage > 0 && damage_sound != nullptr) {
        damage_sound->play();
    }

    if (health <= 0) {
        health = 0;
        die(true);
        return;
    }


    float final_knockback = damage * knockback;
    knockback_velocity = knockback_direction * final_knockback;



    float final_stun = damage * stun_scale;

    if (combat_state != CombatState::BLOCKING && combat_state != CombatState::STUNNED && final_stun > 0.0f) {

        stun_timer = final_stun;
        stun();
    }
}









void Player::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Input *input = Input::get_singleton();

    move_speed_negation = 1.0f;


    if (combat_state == CombatState::STUNNED) {
        hitbox->set_monitoring(false);

        stun_timer -= delta;

        if (stun_timer <= 0.0f) {
            stun_timer = 0.0f;
            recover_from_stun();
        }

        knockback_velocity = knockback_velocity.move_toward(Vector2(0, 0), 500.0f * delta);

        set_velocity(knockback_velocity);
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
                        actor->take_damage(get_attack_damage(), current_weapon->get_stun_scale(), current_weapon->get_knockback(), this);

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

    if (input->is_action_just_pressed("heal")) {
    heal();
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


    if (combat_state != CombatState::ATTACKING && direction.x < 0) {
        animated_sprite->set_flip_h(true);

        Vector2 hitbox_position = hitbox_default_position;
        hitbox_position.x = -hitbox_default_position.x;
        hitbox->set_position(hitbox_position);
    }
    else if (combat_state != CombatState::ATTACKING && direction.x > 0) {
        animated_sprite->set_flip_h(false);

        hitbox->set_position(hitbox_default_position);
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


    knockback_velocity = knockback_velocity.move_toward(Vector2(0, 0), 500.0f * delta);

    set_velocity(direction * move_speed * move_speed_negation + knockback_velocity);

    move_and_slide();
}