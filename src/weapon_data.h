#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class WeaponData : public Resource {
    GDCLASS(WeaponData, Resource)

private:
    String weapon_name = "Weapon";

    int damage = 0;

    float block_damage_negation = 0.0f;

    float attack_duration = 0.0f;

    float parry_duration = 0.0f;
    float pre_parry_duration = 0.0f;

    float stun_scale = 0.0f;
    float knockback = 0.0f;


protected:
    static void _bind_methods();


public:

    // WEAPON NAME
    void set_weapon_name(const String &name);
    String get_weapon_name() const;


    // DAMAGE
    void set_damage(int value);
    int get_damage() const;


    // BLOCK
    void set_block_damage_negation(float value);
    float get_block_damage_negation() const;


    // ATTACK
    void set_attack_duration(float value);
    float get_attack_duration() const;


    // PARRY
    void set_parry_duration(float value);
    float get_parry_duration() const;

    void set_pre_parry_duration(float value);
    float get_pre_parry_duration() const;

    void set_stun_scale(float value);
    float get_stun_scale() const;

    void set_knockback(float value);
    float get_knockback() const;
};

}