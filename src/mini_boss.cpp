#include "mini_boss.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;


// ==========================================
// BIND METHODS
// ==========================================

void MiniBoss::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_max_health", "value"), &MiniBoss::set_max_health);

    ClassDB::bind_method(D_METHOD("set_attack_damage", "value"), &MiniBoss::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &MiniBoss::get_attack_damage);

    ClassDB::bind_method(D_METHOD("set_attack_stun_scale", "value"), &MiniBoss::set_attack_stun_scale);
    ClassDB::bind_method(D_METHOD("get_attack_stun_scale"), &MiniBoss::get_attack_stun_scale);

    ClassDB::bind_method(D_METHOD("set_attack_knockback", "value"), &MiniBoss::set_attack_knockback);
    ClassDB::bind_method(D_METHOD("get_attack_knockback"), &MiniBoss::get_attack_knockback);

    ClassDB::bind_method(D_METHOD("set_attack_cooldown", "value"), &MiniBoss::set_attack_cooldown);
    ClassDB::bind_method(D_METHOD("get_attack_cooldown"), &MiniBoss::get_attack_cooldown);

    ClassDB::bind_method(D_METHOD("set_stun_negation", "value"), &MiniBoss::set_stun_negation);
    ClassDB::bind_method(D_METHOD("get_stun_negation"), &MiniBoss::get_stun_negation);

    ClassDB::bind_method(D_METHOD("set_knockback_negation", "value"), &MiniBoss::set_knockback_negation);
    ClassDB::bind_method(D_METHOD("get_knockback_negation"), &MiniBoss::get_knockback_negation);


    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_RANGE, "1,10000,1"), "set_max_health", "get_max_health");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_damage", "get_attack_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_attack_stun_scale", "get_attack_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_knockback", "get_attack_knockback");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_cooldown", PROPERTY_HINT_RANGE, "0.1,10,0.1"), "set_attack_cooldown", "get_attack_cooldown");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stun_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_stun_negation", "get_stun_negation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "knockback_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_knockback_negation", "get_knockback_negation");
}


// ==========================================
// CONSTRUCTOR
// ==========================================

MiniBoss::MiniBoss() {
    max_health = 100;
    health = max_health;
}


MiniBoss::~MiniBoss() {
}


// ==========================================
// READY
// ==========================================

void MiniBoss::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    health = max_health;

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));
    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));
    body_collision = get_node<CollisionShape2D>(NodePath("CollisionShape2D"));

    hitbox->set_monitoring(false);

    attack_cooldown_timer = attack_cooldown;

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}


// ==========================================
// TAKE DAMAGE
// ==========================================

void MiniBoss::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0) {
        return;
    }

    health -= damage;

    if (health < 0) {
        health = 0;
    }

    UtilityFunctions::print("MiniBoss took ", damage, " damage | HP: ", health, "/", max_health);

    if (health <= 0) {
        die(false);
        return;
    }


    float final_stun = damage * stun_scale * (1.0f - stun_negation);
    float final_knockback = damage * knockback * (1.0f - knockback_negation);


    if (attacker != nullptr) {
        Vector2 knockback_direction = (get_global_position() - attacker->get_global_position()).normalized();
        knockback_velocity = knockback_direction * final_knockback;
    }


    if (combat_state == CombatState::IDLE && final_stun > 0.0f) {
        stun_timer = final_stun;
        combat_state = CombatState::STUNNED;

        if (final_stun >= min_stun_animation_duration) {
            Ref<SpriteFrames> sprite_frames = animated_sprite->get_sprite_frames();

            int frame_count = sprite_frames->get_frame_count(StringName("hitted"));
            double animation_fps = sprite_frames->get_animation_speed(StringName("hitted"));

            if (frame_count > 0 && animation_fps > 0.0) {
                double animation_duration = static_cast<double>(frame_count) / animation_fps;

                animated_sprite->set_speed_scale(animation_duration / final_stun);
                animated_sprite->play("hitted");
            }
        }
    }
}


// ==========================================
// DIE
// ==========================================

void MiniBoss::die(bool play_sound) {
    hitbox->set_monitoring(false);
    hurtbox->set_monitorable(false);
    body_collision->set_deferred("disabled", true);

    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("die");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    UtilityFunctions::print("MiniBoss died");
}


// ==========================================
// ATTACK
// ==========================================

void MiniBoss::attack() {
    if (combat_state != CombatState::IDLE) {
        return;
    }

    combat_state = CombatState::ATTACKING;
    attack_has_hit = false;

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("attack");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}


// ==========================================
// PHYSICS PROCESS
// ==========================================

void MiniBoss::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }


    // ==========================================
    // DEAD
    // ==========================================

    if (health <= 0) {
        if (!animated_sprite->is_playing()) {


            // DEBUG ONLY:
            // Revive the MiniBoss after the death animation so combat can be tested repeatedly.
            // Later replace this block with item spawning + queue_free().
            health = max_health;
            hurtbox->set_monitorable(true);
            body_collision->set_deferred("disabled", false);

            combat_state = CombatState::IDLE;
            attack_cooldown_timer = attack_cooldown;
            knockback_velocity = Vector2(0, 0);

            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");

            UtilityFunctions::print("MiniBoss revived for debug | HP: ", health, "/", max_health);
        }

        return;
    }


    // ==========================================
    // STUN
    // ==========================================

    if (combat_state == CombatState::STUNNED) {
        stun_timer -= delta;

        if (stun_timer <= 0.0f) {
            stun_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }
    }


    // ==========================================
    // KNOCKBACK
    // ==========================================

    if (knockback_velocity.length() > 0.0f) {
        set_velocity(knockback_velocity);

        knockback_velocity = knockback_velocity.move_toward(
            Vector2(0, 0),
            500.0f * delta
        );

        if (knockback_velocity.length() < 1.0f) {
            knockback_velocity = Vector2(0, 0);
        }
    }
    else {
        set_velocity(Vector2(0, 0));
    }

    move_and_slide();


    // ==========================================
    // IDLE
    // ==========================================

    if (combat_state == CombatState::IDLE) {
        if (animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");
        }

        attack_cooldown_timer -= delta;

        if (attack_cooldown_timer <= 0.0f) {
            attack();
        }
    }


    // ==========================================
    // ATTACKING
    // ==========================================

    else if (combat_state == CombatState::ATTACKING) {
        int attack_frame = animated_sprite->get_frame();

        if (attack_frame >= 9 && attack_frame <= 12) {
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
                        actor->take_damage(
                            attack_damage,
                            attack_stun_scale,
                            attack_knockback,
                            this
                        );

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


        if (!animated_sprite->is_playing()) {
            hitbox->set_monitoring(false);

            combat_state = CombatState::IDLE;
            attack_cooldown_timer = attack_cooldown;
        }
    }
}


// ==========================================
// SETTERS / GETTERS
// ==========================================

void MiniBoss::set_max_health(int value) {
    max_health = value;
}


void MiniBoss::set_attack_damage(int value) {
    attack_damage = value;
}

int MiniBoss::get_attack_damage() const {
    return attack_damage;
}


void MiniBoss::set_attack_stun_scale(float value) {
    attack_stun_scale = value;
}

float MiniBoss::get_attack_stun_scale() const {
    return attack_stun_scale;
}


void MiniBoss::set_attack_knockback(float value) {
    attack_knockback = value;
}

float MiniBoss::get_attack_knockback() const {
    return attack_knockback;
}


void MiniBoss::set_attack_cooldown(float value) {
    attack_cooldown = value;
}

float MiniBoss::get_attack_cooldown() const {
    return attack_cooldown;
}


void MiniBoss::set_stun_negation(float value) {
    stun_negation = value;
}

float MiniBoss::get_stun_negation() const {
    return stun_negation;
}


void MiniBoss::set_knockback_negation(float value) {
    knockback_negation = value;
}

float MiniBoss::get_knockback_negation() const {
    return knockback_negation;
}
