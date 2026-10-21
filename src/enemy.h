#pragma once

#include "actor.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/variant/vector2.hpp>


namespace godot {

class Enemy : public Actor {
    GDCLASS(Enemy, Actor)

private:
    // Các thuộc tính có thể chỉnh trong Inspector.
    float detection_range = 300.0f;
    float close_range = 80.0f;
    int attack_damage = 15;
    float attack_stun_scale = 0.1f;
    float attack_knockback = 15.0f;
    float attack_cooldown = 2.0f;

    float stun_negation = 0.5f;
    float knockback_negation = 0.5f;


    // Dữ liệu cần thiết khi chạy AI.
    Actor *target = nullptr;
    AnimatedSprite2D *animated_sprite = nullptr;
    Area2D *hitbox = nullptr;
    Area2D *hurtbox = nullptr;
    CollisionShape2D *body_collision = nullptr;
    AudioStreamPlayer2D *sword_slash_sound = nullptr;
    bool attack_sound_played = false;

    Vector2 hitbox_default_position = Vector2(0, 0);
    Vector2 knockback_velocity = Vector2(0, 0);

    float stun_timer = 0.0f;
    float attack_cooldown_timer = 0.0f;
    bool attack_has_hit = false;

    void update_facing(float direction_x);
    void find_target();
    void update_ai(double delta);
    void update_attack();

protected:
    static void _bind_methods();

public:
    Enemy();
    ~Enemy();

    void _ready() override;
    void _physics_process(double delta) override;

    void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;
    void die(bool play_sound) override;

    void attack();

    // Setters / getters
    void set_move_speed(float value);
    float get_move_speed() const;

    void set_detection_range(float value);
    float get_detection_range() const;

    void set_close_range(float value);
    float get_close_range() const;

    void set_max_health(int value);
    int get_max_health() const;

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
};

}
