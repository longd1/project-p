#include "boss.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;


// ==========================================
// BIND METHODS
// ==========================================

void Boss::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_move_speed", "value"), &Boss::set_move_speed);
    ClassDB::bind_method(D_METHOD("get_move_speed"), &Boss::get_move_speed);

    ClassDB::bind_method(D_METHOD("set_max_health", "value"), &Boss::set_max_health);

    ClassDB::bind_method(D_METHOD("set_attack_damage", "value"), &Boss::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &Boss::get_attack_damage);

    ClassDB::bind_method(D_METHOD("set_attack_stun_scale", "value"), &Boss::set_attack_stun_scale);
    ClassDB::bind_method(D_METHOD("get_attack_stun_scale"), &Boss::get_attack_stun_scale);

    ClassDB::bind_method(D_METHOD("set_attack_knockback", "value"), &Boss::set_attack_knockback);
    ClassDB::bind_method(D_METHOD("get_attack_knockback"), &Boss::get_attack_knockback);

    ClassDB::bind_method(D_METHOD("set_attack_cooldown", "value"), &Boss::set_attack_cooldown);
    ClassDB::bind_method(D_METHOD("get_attack_cooldown"), &Boss::get_attack_cooldown);

    ClassDB::bind_method(D_METHOD("set_stun_negation", "value"), &Boss::set_stun_negation);
    ClassDB::bind_method(D_METHOD("get_stun_negation"), &Boss::get_stun_negation);

    ClassDB::bind_method(D_METHOD("set_knockback_negation", "value"), &Boss::set_knockback_negation);
    ClassDB::bind_method(D_METHOD("get_knockback_negation"), &Boss::get_knockback_negation);

    ClassDB::bind_method(D_METHOD("set_close_range", "value"), &Boss::set_close_range);
    ClassDB::bind_method(D_METHOD("get_close_range"), &Boss::get_close_range);

    ClassDB::bind_method(D_METHOD("set_mid_range", "value"), &Boss::set_mid_range);
    ClassDB::bind_method(D_METHOD("get_mid_range"), &Boss::get_mid_range);

    ClassDB::bind_method(D_METHOD("set_far_range", "value"), &Boss::set_far_range);
    ClassDB::bind_method(D_METHOD("get_far_range"), &Boss::get_far_range);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_speed", PROPERTY_HINT_RANGE, "0,1000,1"), "set_move_speed", "get_move_speed");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_RANGE, "1,10000,1"), "set_max_health", "get_max_health");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_damage", "get_attack_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_attack_stun_scale", "get_attack_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_knockback", "get_attack_knockback");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_cooldown", PROPERTY_HINT_RANGE, "0.1,10,0.1"), "set_attack_cooldown", "get_attack_cooldown");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stun_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_stun_negation", "get_stun_negation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "knockback_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_knockback_negation", "get_knockback_negation");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "close_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_close_range", "get_close_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mid_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_mid_range", "get_mid_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "far_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_far_range", "get_far_range");
}


// ==========================================
// CONSTRUCTOR
// ==========================================

Boss::Boss() {
    max_health = 500;
    health = max_health;
    move_speed = 80.0f;
}

Boss::~Boss() {
}


// ==========================================
// READY
// ==========================================

void Boss::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    health = max_health;

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));
    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));
    body_collision = get_node<CollisionShape2D>(NodePath("CollisionShape2D"));

    hitbox_default_position = hitbox->get_position();
    hitbox->set_monitoring(false);

    find_target();

    random_manager = get_node<Node>(NodePath("/root/RandomManager"));

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}


// ==========================================
// FIND TARGET
// ==========================================

void Boss::find_target() {
    target = Object::cast_to<Actor>(get_tree()->get_first_node_in_group("player"));
}


// ==========================================
// PHYSICS PROCESS
// ==========================================

void Boss::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // AI sẽ thêm sau.
}


// ==========================================
// SETTERS / GETTERS
// ==========================================

void Boss::set_move_speed(float value) { move_speed = value; }
float Boss::get_move_speed() const { return move_speed; }

void Boss::set_max_health(int value) { max_health = value; }

void Boss::set_attack_damage(int value) { attack_damage = value; }
int Boss::get_attack_damage() const { return attack_damage; }

void Boss::set_attack_stun_scale(float value) { attack_stun_scale = value; }
float Boss::get_attack_stun_scale() const { return attack_stun_scale; }

void Boss::set_attack_knockback(float value) { attack_knockback = value; }
float Boss::get_attack_knockback() const { return attack_knockback; }

void Boss::set_attack_cooldown(float value) { attack_cooldown = value; }
float Boss::get_attack_cooldown() const { return attack_cooldown; }

void Boss::set_stun_negation(float value) { stun_negation = value; }
float Boss::get_stun_negation() const { return stun_negation; }

void Boss::set_knockback_negation(float value) { knockback_negation = value; }
float Boss::get_knockback_negation() const { return knockback_negation; }

void Boss::set_close_range(float value) { close_range = value; }
float Boss::get_close_range() const { return close_range; }

void Boss::set_mid_range(float value) { mid_range = value; }
float Boss::get_mid_range() const { return mid_range; }

void Boss::set_far_range(float value) { far_range = value; }
float Boss::get_far_range() const { return far_range; }