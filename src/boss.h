#pragma once

#include "actor.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class Boss : public Actor {
    GDCLASS(Boss, Actor)

private:
    // ==========================================
    // ATTACK STATS
    // ==========================================

    int attack_damage = 20;
    float attack_stun_scale = 0.1f;
    float attack_knockback = 20.0f;
    float attack_cooldown = 2.0f;


    // ==========================================
    // AI RANGE
    // ==========================================

    float close_range = 80.0f;
    float mid_range = 180.0f;
    float far_range = 300.0f;

    Actor *target = nullptr;
    Node *random_manager = nullptr;


    // ==========================================
    // DEFENSE
    // ==========================================

    float stun_negation = 0.7f;
    float knockback_negation = 0.7f;


    // ==========================================
    // RUNTIME
    // ==========================================

    float stun_timer = 0.0f;
    float attack_cooldown_timer = 0.0f;

    Vector2 knockback_velocity = Vector2(0, 0);

    bool attack_has_hit = false;


    // ==========================================
    // NODES
    // ==========================================

    AnimatedSprite2D *animated_sprite = nullptr;

    Area2D *hitbox = nullptr;
    Area2D *hurtbox = nullptr;

    CollisionShape2D *body_collision = nullptr;

    Vector2 hitbox_default_position = Vector2(0, 0);


    // ==========================================
    // INTERNAL
    // ==========================================

    void find_target();
    void decide_action();
    void update_facing(float direction_x);


protected:
    static void _bind_methods();


public:
    Boss();
    ~Boss();

    void _ready() override;
    void _physics_process(double delta) override;

    //void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;
    //void die(bool play_sound) override;

    void attack();


    // ==========================================
    // SETTERS / GETTERS
    // ==========================================

    void set_move_speed(float value);
    float get_move_speed() const;

    void set_max_health(int value);

    void set_attack_damage(int value);
    int get_attack_damage() const;

    void set_attack_stun_scale(float value);
    float get_attack_stun_scale() const;

    void set_attack_knockback(float value);
    float get_attack_knockback() const;

    void set_attack_cooldown(float value);
    float get_attack_cooldown() const;

    void set_stun_negation(float value);
    float get_stun_negation() const;

    void set_knockback_negation(float value);
    float get_knockback_negation() const;

    void set_close_range(float value);
    float get_close_range() const;

    void set_mid_range(float value);
    float get_mid_range() const;

    void set_far_range(float value);
    float get_far_range() const;
};

}