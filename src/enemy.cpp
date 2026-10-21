#include "enemy.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

using namespace godot;

Enemy::Enemy() {
    max_health = 100;
    health = max_health;
    move_speed = 80.0f;
}

Enemy::~Enemy() {
}

void Enemy::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_move_speed", "value"), &Enemy::set_move_speed);
    ClassDB::bind_method(D_METHOD("get_move_speed"), &Enemy::get_move_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_speed", PROPERTY_HINT_RANGE, "0,1000,1"), "set_move_speed", "get_move_speed");

    ClassDB::bind_method(D_METHOD("set_detection_range", "value"), &Enemy::set_detection_range);
    ClassDB::bind_method(D_METHOD("get_detection_range"), &Enemy::get_detection_range);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "detection_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_detection_range", "get_detection_range");

    ClassDB::bind_method(D_METHOD("set_close_range", "value"), &Enemy::set_close_range);
    ClassDB::bind_method(D_METHOD("get_close_range"), &Enemy::get_close_range);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "close_range", PROPERTY_HINT_RANGE, "0,1000,1"), "set_close_range", "get_close_range");

    ClassDB::bind_method(
    D_METHOD("set_attack_y_tolerance", "value"),
    &Enemy::set_attack_y_tolerance
    );
    ClassDB::bind_method(
    D_METHOD("get_attack_y_tolerance"),
    &Enemy::get_attack_y_tolerance
    );

    ADD_PROPERTY(
    PropertyInfo(
        Variant::FLOAT,
        "attack_y_tolerance",
        PROPERTY_HINT_RANGE,
        "0,200,1"
    ),
    "set_attack_y_tolerance",
    "get_attack_y_tolerance"
    );

    ClassDB::bind_method(D_METHOD("set_max_health", "value"), &Enemy::set_max_health);
    ClassDB::bind_method(D_METHOD("get_max_health"), &Enemy::get_max_health);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_RANGE, "1,10000,1"), "set_max_health", "get_max_health");

    ClassDB::bind_method(D_METHOD("set_attack_damage", "value"), &Enemy::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &Enemy::get_attack_damage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_damage", "get_attack_damage");

    ClassDB::bind_method(D_METHOD("set_attack_stun_scale", "value"), &Enemy::set_attack_stun_scale);
    ClassDB::bind_method(D_METHOD("get_attack_stun_scale"), &Enemy::get_attack_stun_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_attack_stun_scale", "get_attack_stun_scale");

    ClassDB::bind_method(D_METHOD("set_attack_knockback", "value"), &Enemy::set_attack_knockback);
    ClassDB::bind_method(D_METHOD("get_attack_knockback"), &Enemy::get_attack_knockback);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_knockback", "get_attack_knockback");

    ClassDB::bind_method(D_METHOD("set_attack_cooldown", "value"), &Enemy::set_attack_cooldown);
    ClassDB::bind_method(D_METHOD("get_attack_cooldown"), &Enemy::get_attack_cooldown);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_cooldown", PROPERTY_HINT_RANGE, "0,10,0.1"), "set_attack_cooldown", "get_attack_cooldown");

    ClassDB::bind_method(D_METHOD("set_stun_negation", "value"), &Enemy::set_stun_negation);
    ClassDB::bind_method(D_METHOD("get_stun_negation"), &Enemy::get_stun_negation);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stun_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_stun_negation", "get_stun_negation");

    ClassDB::bind_method(D_METHOD("set_knockback_negation", "value"), &Enemy::set_knockback_negation);
    ClassDB::bind_method(D_METHOD("get_knockback_negation"), &Enemy::get_knockback_negation);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "knockback_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_knockback_negation", "get_knockback_negation");

    ClassDB::bind_method(D_METHOD("attack"), &Enemy::attack);
}


void Enemy::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    health = max_health;

    animated_sprite = Object::cast_to<AnimatedSprite2D>(
        get_node_or_null(NodePath("AnimatedSprite2D"))
    );

    hitbox = Object::cast_to<Area2D>(
        get_node_or_null(NodePath("hitbox/Area2D"))
    );

    hurtbox = Object::cast_to<Area2D>(
        get_node_or_null(NodePath("hurtbox/Area2D"))
    );

    sword_slash_sound = Object::cast_to<AudioStreamPlayer2D>(
        get_node_or_null(NodePath("sound/swordslash"))
    );

    body_collision = Object::cast_to<CollisionShape2D>(
        get_node_or_null(NodePath("CollisionShape2D"))
    );

    if (hitbox != nullptr) {
        hitbox_default_position = hitbox->get_position();
        hitbox->set_monitoring(false);
    }

    find_target();

    if (animated_sprite != nullptr) {
        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("idle");
    }

    if (animated_sprite == nullptr) {
        UtilityFunctions::push_warning(
        "Enemy: AnimatedSprite2D not found."
        );
    }

    if (hitbox == nullptr) {
        UtilityFunctions::push_warning(
        "Enemy: hitbox/Area2D not found."
        );
    }

    if (hurtbox == nullptr) {
        UtilityFunctions::push_warning(
        "Enemy: hurtbox/Area2D not found."
        );
    }

    if (sword_slash_sound == nullptr) {
        UtilityFunctions::push_warning(
        "Enemy: sound/swordsplash not found."
        );
    }
}


void Enemy::find_target() {
    if (get_tree() == nullptr) {
        target = nullptr;
        return;
    }

    Node *player_node = get_tree()->get_first_node_in_group("player");
    target = Object::cast_to<Actor>(player_node);
}

void Enemy::update_facing(float direction_x) {
    if (animated_sprite == nullptr || direction_x == 0.0f) return;

    bool face_left = direction_x < 0.0f;

    animated_sprite->set_flip_h(face_left);

    if (hitbox != nullptr) {
        Vector2 position = hitbox_default_position;
        if (face_left) position.x = -position.x;
        hitbox->set_position(position);

        Vector2 scale = hitbox->get_scale();
        scale.x = Math::abs(scale.x) * (face_left ? -1.0f : 1.0f);
        hitbox->set_scale(scale);
    }
}

void Enemy::attack() {
    if (health <= 0 ||
        combat_state == CombatState::ATTACKING ||
        combat_state == CombatState::STUNNED ||
        attack_cooldown_timer > 0.0f) {
        return;
    }

    if (animated_sprite == nullptr) {
        return;
    }

    combat_state = CombatState::ATTACKING;
    attack_has_hit = false;
    attack_sound_played = false;

    set_velocity(Vector2());

    // Theo dõi overlap từ đầu animation.
    if (hitbox != nullptr) {
        hitbox->set_monitoring(true);
    }

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("attack");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}

void Enemy::update_ai(double delta) {
    if (attack_cooldown_timer > 0.0f) {
        attack_cooldown_timer = MAX(0.0f, attack_cooldown_timer - static_cast<float>(delta));
    }

    find_target();

    // Xử lý stun trước khi kiểm tra target.
   // Enemy vẫn phải hồi stun kể cả khi Player bị xóa khỏi scene.
    if (combat_state == CombatState::STUNNED) {
        stun_timer -= static_cast<float>(delta);

        set_velocity(knockback_velocity);

    if (stun_timer <= 0.0f) {
        stun_timer = 0.0f;
        combat_state = CombatState::IDLE;
        set_velocity(Vector2());

        if (animated_sprite != nullptr) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");
        }
    }

    return;
}

    if (target == nullptr) {
        combat_state = CombatState::IDLE;
        set_velocity(Vector2());
        if (animated_sprite != nullptr && animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->play("idle");
        }
        return;
    }

    const Vector2 enemy_position = get_global_position();
    const Vector2 player_position = target->get_global_position();

    const float delta_x = player_position.x - enemy_position.x;
    const float delta_y = player_position.y - enemy_position.y;

    const float distance = enemy_position.distance_to(player_position);

    update_facing(delta_x);

    if (combat_state == CombatState::ATTACKING) {
        set_velocity(Vector2());
        update_attack();
        return;
    }

    if (distance > detection_range) {
        combat_state = CombatState::IDLE;
        set_velocity(Vector2());
        if (animated_sprite != nullptr && animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->play("idle");
        }
        return;
    }

    const bool in_attack_range =
    Math::abs(delta_x) <= close_range &&
    Math::abs(delta_y) <= attack_y_tolerance;

    if (in_attack_range) {
       set_velocity(Vector2());

       if (attack_cooldown_timer <= 0.0f) {
           attack();
        } else if (animated_sprite != nullptr &&
               animated_sprite->get_animation() != StringName("idle")) {
               animated_sprite->set_speed_scale(1.0f);
               animated_sprite->play("idle");
        }

       return;
    }

    combat_state = CombatState::MOVING;

    Vector2 direction = (player_position - enemy_position).normalized();

    set_velocity(direction * move_speed);

    if (animated_sprite != nullptr && animated_sprite->get_animation() != StringName("run")) {
        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("run");
    }
}

void Enemy::update_attack() {
    if (animated_sprite == nullptr || hitbox == nullptr) {
        if (hitbox != nullptr) {
            hitbox->set_monitoring(false);
        }

        combat_state = CombatState::IDLE;
        attack_cooldown_timer = attack_cooldown;
        return;
    }

    const int frame = animated_sprite->get_frame();
    const bool hit_window = (frame >= 4 && frame <= 5);

    // Phát tiếng chém một lần trong mỗi đòn đánh.
    if (frame >= 4 && !attack_sound_played) {
        if (sword_slash_sound != nullptr &&
            sword_slash_sound->get_stream().is_valid()) {
            sword_slash_sound->play();
        } else {
            UtilityFunctions::push_warning(
                "Enemy: sword slash sound node/stream is missing."
            );
        }

        attack_sound_played = true;
    }

    // Chỉ gây sát thương trong hit window.
    if (hit_window && !attack_has_hit && target != nullptr) {
        TypedArray<Area2D> areas = hitbox->get_overlapping_areas();

        for (int i = 0; i < areas.size(); ++i) {
            Area2D *area = Object::cast_to<Area2D>(areas[i]);

            if (area == nullptr) {
                continue;
            }

            Node *node = area;

            while (node != nullptr) {
                Actor *actor = Object::cast_to<Actor>(node);

                if (actor == target && actor != this) {
                    actor->take_damage(
                        attack_damage,
                        attack_stun_scale,
                        attack_knockback,
                        this
                    );

                    attack_has_hit = true;
                    break;
                }

                node = node->get_parent();
            }

            if (attack_has_hit) {
                break;
            }
        }
    }

    // Chỉ tắt hitbox khi animation thực sự kết thúc.
    if (!animated_sprite->is_playing()) {
        hitbox->set_monitoring(false);

        combat_state = CombatState::IDLE;
        attack_cooldown_timer = attack_cooldown;

        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("idle");
    }
}

void Enemy::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (health <= 0) {
        if (animated_sprite == nullptr || !animated_sprite->is_playing()) {
            queue_free();
        }
        return;
    }

    update_ai(delta);

    // Khi không bị stun, cộng knockback vào vận tốc AI.
    // Khi bị stun, update_ai() đã đặt velocity = knockback_velocity.
    if (combat_state != CombatState::STUNNED &&
        knockback_velocity.length() > 0.0f) {
        set_velocity(get_velocity() + knockback_velocity);
    }

    move_and_slide();

    // Giảm knockback sau khi đã sử dụng nó để di chuyển.
    if (knockback_velocity.length() > 0.0f) {
        knockback_velocity = knockback_velocity.move_toward(Vector2(), 500.0f * static_cast<float>(delta));

    if (knockback_velocity.length() < 1.0f) {
        knockback_velocity = Vector2();
        }
    }
}

void Enemy::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0) {
        return;
    }

    Actor::take_damage(damage, stun_scale, knockback, attacker);

    if (health <= 0) {
        die(false);
        return;
    }

    const float final_stun = damage * stun_scale * (1.0f - stun_negation);
    const float final_knockback = damage * knockback * (1.0f - knockback_negation);

    if (attacker != nullptr) {
        Vector2 direction = (get_global_position() - attacker->get_global_position()).normalized();
        knockback_velocity = direction * final_knockback;
    }

    if ((combat_state == CombatState::IDLE || combat_state == CombatState::MOVING) &&
        final_stun > 0.0f) {
        stun_timer = final_stun;
        combat_state = CombatState::STUNNED;

        if (hitbox != nullptr) {
            hitbox->set_monitoring(false);
        }

        if (animated_sprite != nullptr) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("hitted");
        }
    }
}

void Enemy::die(bool play_sound) {
    if (health > 0) {
        health = 0;
    }

    set_velocity(Vector2());
    knockback_velocity = Vector2();

    if (hitbox != nullptr) {
        hitbox->set_monitoring(false);
    }
    if (hurtbox != nullptr) {
        hurtbox->set_monitorable(false);
    }
    if (body_collision != nullptr) {
        body_collision->set_deferred("disabled", true);
    }

    if (animated_sprite != nullptr) {
        animated_sprite->stop();
        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("die");
        animated_sprite->set_frame_and_progress(0, 0.0f);
    } else {
        queue_free();
    }

    UtilityFunctions::print("Enemy died");
}

// Setters / getters

void Enemy::set_move_speed(float value) { move_speed = MAX(0.0f, value); }
float Enemy::get_move_speed() const { return move_speed; }

void Enemy::set_detection_range(float value) { detection_range = MAX(0.0f, value); }
float Enemy::get_detection_range() const { return detection_range; }

void Enemy::set_close_range(float value) { close_range = MAX(0.0f, value); }
float Enemy::get_close_range() const { return close_range; }

void Enemy::set_max_health(int value) {
    max_health = MAX(1, value);
    if (health > max_health) {
        health = max_health;
    }
}
int Enemy::get_max_health() const { return max_health; }

void Enemy::set_attack_damage(int value) { attack_damage = MAX(0, value); }
int Enemy::get_attack_damage() const { return attack_damage; }

void Enemy::set_attack_stun_scale(float value) { attack_stun_scale = MAX(0.0f, value); }
float Enemy::get_attack_stun_scale() const { return attack_stun_scale; }

void Enemy::set_attack_knockback(float value) { attack_knockback = MAX(0.0f, value); }
float Enemy::get_attack_knockback() const { return attack_knockback; }

void Enemy::set_attack_cooldown(float value) { attack_cooldown = MAX(0.0f, value); }
float Enemy::get_attack_cooldown() const { return attack_cooldown; }

void Enemy::set_stun_negation(float value) { stun_negation = CLAMP(value, 0.0f, 1.0f); }
float Enemy::get_stun_negation() const { return stun_negation; }

void Enemy::set_knockback_negation(float value) { knockback_negation = CLAMP(value, 0.0f, 1.0f); }
float Enemy::get_knockback_negation() const { return knockback_negation; }

void Enemy::set_attack_y_tolerance(float value) {
    attack_y_tolerance = MAX(0.0f, value);
}

float Enemy::get_attack_y_tolerance() const {
    return attack_y_tolerance;
}