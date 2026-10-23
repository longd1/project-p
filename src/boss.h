#pragma once

#include "actor.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/variant/string_name.hpp>
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

    const float attack_align_tolerance = 5.0f;

    // ==========================================
    // SKILL 1
    // ==========================================

    int skill1_damage = 20;
    float skill1_stun_scale = 0.1f;
    float skill1_knockback_multiplier = 0.5f;

    bool skill1_hitbox1_first_hit = false;
    bool skill1_hitbox1_second_hit = false;
    bool skill1_hitbox2_first_hit = false;
    bool skill1_hitbox2_second_hit = false;

    // ==========================================
    // SKILL 2
    // ==========================================

    int skill2_damage = 20;
    float skill2_stun_scale = 0.1f;
    float skill2_knockback_multiplier = 1.0f;

    bool skill2_first_hit = false;
    bool skill2_second_hit = false;

    // ==========================================
    // SKILL 3
    // ==========================================

    int skill3_damage = 20;
    float skill3_stun_scale = 0.1f;
    float skill3_knockback_multiplier = 1.0f;

    bool skill3_first_hit = false;
    bool skill3_second_hit = false;

    // ==========================================
    // SKILL 4
    // ==========================================

    int skill4_damage = 20;
    float skill4_stun_scale = 0.1f;
    float skill4_knockback_multiplier = 1.0f;

    bool skill4_first_hit = false;
    bool skill4_second_hit = false;

    // ==========================================
    // SKILL 5
    // ==========================================

    int skill5_damage = 20;
    float skill5_stun_scale = 0.1f;
    float skill5_knockback_multiplier = 1.0f;

    bool skill5_first_hit = false;
    bool skill5_second_hit = false;

    // ==========================================
    // AI RANGE
    // ==========================================

    float close_range = 80.0f;
    float mid_range = 180.0f;
    float far_range = 300.0f;

    Actor *target = nullptr;
    Node *random_manager = nullptr;

    // 0 = không khóa movement
    // 1 = đang tiến gần từ FAR
    // 2 = đang tiến gần từ MID
    int moving_from_range = 0;

    // ==========================================
    // DEFENSE
    // ==========================================

    float damage_negation = 0.4f;
    float stun_negation = 0.7f;
    float knockback_negation = 0.7f;

    int parry_chance = 8;
    bool is_parrying = false;

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

    Area2D *skill1_hitbox1 = nullptr;
    Area2D *skill1_hitbox2 = nullptr;

    Area2D *skill2_hitbox = nullptr;
    Area2D *skill3_hitbox = nullptr;

    Area2D *skill4_hitbox1 = nullptr;
    Area2D *skill4_hitbox2 = nullptr;

    Area2D *skill5_hitbox1 = nullptr;
    Area2D *skill5_hitbox2 = nullptr;

    Area2D *hurtbox = nullptr;

    AnimatedSprite2D *parry_effect = nullptr;
    Node *parry_sound = nullptr;

    AudioStreamPlayer2D *attack_sound = nullptr;
    AudioStreamPlayer2D *skill1_sound = nullptr;
    AudioStreamPlayer2D *skill2_sound = nullptr;
    AudioStreamPlayer2D *skill3_sound = nullptr;
    AudioStreamPlayer2D *skill4_sound = nullptr;
    AudioStreamPlayer2D *skill5_sound = nullptr;

    CollisionShape2D *body_collision = nullptr;

    Vector2 animated_sprite_default_position = Vector2(0, 0);
    Vector2 animated_sprite_default_scale = Vector2(1, 1);

    Vector2 parry_effect_default_position = Vector2(0, 0);
    Vector2 parry_effect_default_scale = Vector2(1, 1);

    Vector2 hitbox_default_position = Vector2(0, 0);
    Vector2 hitbox_default_scale = Vector2(1, 1);

    Vector2 skill1_hitbox1_default_position = Vector2(0, 0);
    Vector2 skill1_hitbox1_default_scale = Vector2(1, 1);

    Vector2 skill1_hitbox2_default_position = Vector2(0, 0);
    Vector2 skill1_hitbox2_default_scale = Vector2(1, 1);

    Vector2 skill2_hitbox_default_position = Vector2(0, 0);
    Vector2 skill2_hitbox_default_scale = Vector2(1, 1);

    Vector2 skill3_hitbox_default_position = Vector2(0, 0);
    Vector2 skill3_hitbox_default_scale = Vector2(1, 1);

    Vector2 skill4_hitbox1_default_position = Vector2(0, 0);
    Vector2 skill4_hitbox1_default_scale = Vector2(1, 1);

    Vector2 skill4_hitbox2_default_position = Vector2(0, 0);
    Vector2 skill4_hitbox2_default_scale = Vector2(1, 1);

    Vector2 skill5_hitbox1_default_position = Vector2(0, 0);
    Vector2 skill5_hitbox1_default_scale = Vector2(1, 1);

    Vector2 skill5_hitbox2_default_position = Vector2(0, 0);
    Vector2 skill5_hitbox2_default_scale = Vector2(1, 1);

    // ==========================================
    // INTERNAL
    // ==========================================

    void find_target();
    void decide_action();
    void update_facing(float direction_x);

    void try_parry();
    void update_attack();
    void disable_attack_hitboxes();

    void apply_attack_hit(Area2D *active_hitbox, bool &has_hit, int damage, float stun_scale, float knockback);

protected:
    static void _bind_methods();

public:
    Boss();
    ~Boss();

    void _ready() override;
    void _physics_process(double delta) override;

    void attack();
    void skill_1();
    void skill_2();
    void skill_3();
    void skill_4();
    void skill_5();
    void parry();
    void die(bool play_sound = true) override;

    void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;

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

    void set_damage_negation(float value);
    float get_damage_negation() const;

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

    void set_skill1_damage(int value);
    int get_skill1_damage() const;

    void set_skill1_stun_scale(float value);
    float get_skill1_stun_scale() const;

    void set_skill1_knockback_multiplier(float value);
    float get_skill1_knockback_multiplier() const;

    void set_skill2_damage(int value);
    int get_skill2_damage() const;

    void set_skill2_stun_scale(float value);
    float get_skill2_stun_scale() const;

    void set_skill2_knockback_multiplier(float value);
    float get_skill2_knockback_multiplier() const;

    void set_skill3_damage(int value);
    int get_skill3_damage() const;

    void set_skill3_stun_scale(float value);
    float get_skill3_stun_scale() const;

    void set_skill3_knockback_multiplier(float value);
    float get_skill3_knockback_multiplier() const;

    void set_skill4_damage(int value);
    int get_skill4_damage() const;

    void set_skill4_stun_scale(float value);
    float get_skill4_stun_scale() const;

    void set_skill4_knockback_multiplier(float value);
    float get_skill4_knockback_multiplier() const;

    void set_skill5_damage(int value);
    int get_skill5_damage() const;

    void set_skill5_stun_scale(float value);
    float get_skill5_stun_scale() const;

    void set_skill5_knockback_multiplier(float value);
    float get_skill5_knockback_multiplier() const;
};

}
