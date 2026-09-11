#pragma once

#include <godot_cpp/classes/character_body2d.hpp>

namespace godot {

class Actor : public CharacterBody2D {
    GDCLASS(Actor, CharacterBody2D)

protected:
    int health = 0;
    int max_health = 0;
    float move_speed = 0.0f;

    static void _bind_methods();

public:
    Actor();
    ~Actor();

    void take_damage(int damage);
    void die();
};

}