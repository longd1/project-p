#pragma once

#include "actor.h"
#include "weapon_data.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/gpu_particles2d.hpp>

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:
    int base_damage = 5;

    Ref<WeaponData> current_weapon;

    float attack_timer = 0.0f;
    bool attack_has_hit = false;

    float parry_timer = 0.0f;
    float stun_timer = 0.0f;
    Vector2 knockback_velocity = Vector2(0, 0);


    AnimatedSprite2D *animated_sprite = nullptr;
    AnimatedSprite2D *effect = nullptr;
    Area2D *hitbox = nullptr;
    Area2D *hurtbox = nullptr;
    AudioStreamPlayer2D *death_sound = nullptr;
    AudioStreamPlayer2D *parry_sound = nullptr;
    AudioStreamPlayer2D *block_sound = nullptr;

    

protected:
    static void _bind_methods();

public:
    Player();
    ~Player();

    void _ready() override;
    void _physics_process(double delta) override;

    void set_current_weapon(const Ref<WeaponData> &weapon);
    Ref<WeaponData> get_current_weapon() const;

    int get_attack_damage() const;
    float get_block_damage_negation() const;

    int current_heal_flasks = 0;

    void die(bool play_sound = true) override;

    void attack();
    void parry();
    void parry_start_up();
    void heal();


    void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;
};

}
