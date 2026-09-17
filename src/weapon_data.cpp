#include "weapon_data.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;


// ==================================================
// BIND METHODS
// ==================================================

void WeaponData::_bind_methods() {

    // WEAPON NAME
    ClassDB::bind_method(
        D_METHOD("set_weapon_name", "name"),
        &WeaponData::set_weapon_name
    );

    ClassDB::bind_method(
        D_METHOD("get_weapon_name"),
        &WeaponData::get_weapon_name
    );


    // DAMAGE
    ClassDB::bind_method(
        D_METHOD("set_damage", "value"),
        &WeaponData::set_damage
    );

    ClassDB::bind_method(
        D_METHOD("get_damage"),
        &WeaponData::get_damage
    );


    // BLOCK DAMAGE NEGATION
    ClassDB::bind_method(
        D_METHOD("set_block_damage_negation", "value"),
        &WeaponData::set_block_damage_negation
    );

    ClassDB::bind_method(
        D_METHOD("get_block_damage_negation"),
        &WeaponData::get_block_damage_negation
    );


    // ATTACK DURATION
    ClassDB::bind_method(
        D_METHOD("set_attack_duration", "value"),
        &WeaponData::set_attack_duration
    );

    ClassDB::bind_method(
        D_METHOD("get_attack_duration"),
        &WeaponData::get_attack_duration
    );


    // PARRY DURATION
    ClassDB::bind_method(
        D_METHOD("set_parry_duration", "value"),
        &WeaponData::set_parry_duration
    );

    ClassDB::bind_method(
        D_METHOD("get_parry_duration"),
        &WeaponData::get_parry_duration
    );


    // PRE PARRY DURATION
    ClassDB::bind_method(
        D_METHOD("set_pre_parry_duration", "value"),
        &WeaponData::set_pre_parry_duration
    );

    ClassDB::bind_method(
        D_METHOD("get_pre_parry_duration"),
        &WeaponData::get_pre_parry_duration
    );


    // ==================================================
    // PROPERTIES
    // ==================================================

    ADD_PROPERTY(
        PropertyInfo(
            Variant::STRING,
            "weapon_name"
        ),
        "set_weapon_name",
        "get_weapon_name"
    );


    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "damage"
        ),
        "set_damage",
        "get_damage"
    );


    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "block_damage_negation"
        ),
        "set_block_damage_negation",
        "get_block_damage_negation"
    );


    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "attack_duration"
        ),
        "set_attack_duration",
        "get_attack_duration"
    );


    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "parry_duration"
        ),
        "set_parry_duration",
        "get_parry_duration"
    );


    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "pre_parry_duration"
        ),
        "set_pre_parry_duration",
        "get_pre_parry_duration"
    );
}


// ==================================================
// WEAPON NAME
// ==================================================

void WeaponData::set_weapon_name(const String &name) {
    weapon_name = name;
}

String WeaponData::get_weapon_name() const {
    return weapon_name;
}


// ==================================================
// DAMAGE
// ==================================================

void WeaponData::set_damage(int value) {
    damage = value;
}

int WeaponData::get_damage() const {
    return damage;
}


// ==================================================
// BLOCK DAMAGE NEGATION
// ==================================================

void WeaponData::set_block_damage_negation(float value) {
    block_damage_negation = value;
}

float WeaponData::get_block_damage_negation() const {
    return block_damage_negation;
}


// ==================================================
// ATTACK DURATION
// ==================================================

void WeaponData::set_attack_duration(float value) {
    attack_duration = value;
}

float WeaponData::get_attack_duration() const {
    return attack_duration;
}


// ==================================================
// PARRY DURATION
// ==================================================

void WeaponData::set_parry_duration(float value) {
    parry_duration = value;
}

float WeaponData::get_parry_duration() const {
    return parry_duration;
}


// ==================================================
// PRE PARRY DURATION
// ==================================================

void WeaponData::set_pre_parry_duration(float value) {
    pre_parry_duration = value;
}

float WeaponData::get_pre_parry_duration() const {
    return pre_parry_duration;
}