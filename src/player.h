#pragma once

#include "actor.h"
#include "weapon_data.h"

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:

    // ==========================================
    // PLAYER STATS
    // ==========================================

    int max_mana = 100;
    int mana = max_mana;

    int base_damage = 5;


    // ==========================================
    // WEAPON
    // ==========================================

    Ref<WeaponData> current_weapon;


    // ==========================================
    // ATTACK
    // ==========================================

    float attack_timer = 0.0f;


    // ==========================================
    // PARRY
    // ==========================================

    float parry_timer = 0.0f;


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


    // ==========================================
    // ATTACK
    // ==========================================

    void attack();


    // ==========================================
    // PARRY
    // ==========================================

    void parry();
    void parry_start_up();


    // ==========================================
    // BLOCK
    // ==========================================

    void block();
};

}