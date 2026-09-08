#pragma once

#include <godot_cpp/classes/character_body2d.hpp>

namespace godot {

class Actor : public CharacterBody2D {
    GDCLASS(Actor, CharacterBody2D)

protected:
    int health;
    int mana;

    static void _bind_methods();

public:
    Actor();
    ~Actor();

    void take_damage(int damage);
    void die();
};

}