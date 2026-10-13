#pragma once

#include "actor.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class MiniBoss : public Actor {
    GDCLASS(MiniBoss, Actor)

private:
    // ==========================================
    // ATTACK STATS
    // ==========================================

    int attack_damage = 15;
    float attack_stun_scale = 0.1f;
    float attack_knockback = 15.0f;
    float attack_cooldown = 2.0f;
    float buff_damage_multiplier = 1.5f;
    int heal_amount = 30;

    bool is_buffed = false;
    bool buff_has_applied = false;
    bool heal_has_applied = false;
    bool has_healed = false;
    

    Node *random_manager = nullptr;

    // SLIDE
    float slide_speed = 220.0f;
    int slide_damage = 5;
    float slide_stun_scale = 0.2f;
    float slide_knockback = 5.0f;
    float slide_target_x = 0.0f;

    bool slide_aligned = false;
    bool slide_has_hit = false;
    float slide_direction = 0.0f;

    const float slide_align_tolerance = 5.0f;




    // ==========================================
    // AI
    // ==========================================

    float detection_range = 300.0f;
    float close_range = 80.0f;
    float mid_range = 180.0f;
    float far_range = 300.0f;


    Actor *target = nullptr;


    // ==========================================
    // DEFENSE
    // ==========================================

    float stun_negation = 0.5f;
    float knockback_negation = 0.5f;

    float min_stun_animation_duration = 0.15f;


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

    

    void update_facing(float direction_x);
    void decide_action();
    


protected:
    static void _bind_methods();


public:
    MiniBoss();
    ~MiniBoss();

    void _ready() override;
    void _physics_process(double delta) override;

    void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;
    void die(bool play_sound) override;

    void attack();

    void buff();
    void heal();
    void start_slide();


    // ==========================================
    // SETTERS / GETTERS
    // ==========================================

    void set_move_speed(float value);
    float get_move_speed() const;

    void set_detection_range(float value);
    float get_detection_range() const;

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

    void set_slide_speed(float value);
    float get_slide_speed() const;

    void set_slide_damage(int value);
    int get_slide_damage() const;

    void set_slide_stun_scale(float value);
    float get_slide_stun_scale() const;

    void set_slide_knockback(float value);
    float get_slide_knockback() const;
    };

}