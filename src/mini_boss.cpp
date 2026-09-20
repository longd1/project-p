#include "mini_boss.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>

using namespace godot;


void MiniBoss::_bind_methods() {
}


MiniBoss::MiniBoss() {
    max_health = 100;
    health = max_health;
}


MiniBoss::~MiniBoss() {
}


void MiniBoss::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");

    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hitbox->set_monitoring(false);
}


void MiniBoss::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    health -= damage;

    if (health < 0) {
        health = 0;
    }

    float final_stun = damage * stun_scale * (1.0f - stun_negation);
    float final_knockback = damage * knockback * (1.0f - knockback_negation);
    Vector2 knockback_direction = (get_global_position() - attacker->get_global_position()).normalized();

    knockback_velocity = knockback_direction * final_knockback;

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







void MiniBoss::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (combat_state == CombatState::STUNNED) {
        stun_timer -= delta;

        if (stun_timer <= 0.0f) {
            stun_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }
    }

    if (knockback_velocity.length() > 0.0f) {
        set_velocity(knockback_velocity);
        knockback_velocity = knockback_velocity.move_toward(Vector2(0, 0), 500.0f * delta);

        if (knockback_velocity.length() < 1.0f) {
            knockback_velocity = Vector2(0, 0);
        }
    }
    else {
        set_velocity(Vector2(0, 0));
    }

    move_and_slide();

    if (combat_state == CombatState::IDLE) {
        if (animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");
        }
    }

    if (combat_state == CombatState::IDLE) {
        attack_cooldown_timer -= delta;

        if (attack_cooldown_timer <= 0.0f) {
            attack();
        }
    }

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
                        actor->take_damage(15, 0.1f, 15.0f, this);

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
            attack_cooldown_timer = 2.0f;
        }
    }
}