#pragma once

#include "actor.h"

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>

namespace godot {

class MiniBoss : public Actor {
    GDCLASS(MiniBoss, Actor)

private:
    float stun_timer = 0.0f;
    Vector2 knockback_velocity = Vector2(0, 0);

    float stun_negation = 0.5f;
    float knockback_negation = 0.5f;

    float min_stun_animation_duration = 0.15f;

    AnimatedSprite2D *animated_sprite = nullptr;

    float attack_cooldown_timer = 2.0f;
    bool attack_has_hit = false;

    Area2D *hitbox = nullptr;


protected:
    static void _bind_methods();

public:
    MiniBoss();
    ~MiniBoss();

    void _ready() override;
    void take_damage(int damage, float stun_scale, float knockback, Actor *attacker) override;
    void _physics_process(double delta) override;
    void attack();
};

}
