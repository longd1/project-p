#include "boss.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/typed_array.hpp>

#include <algorithm>

using namespace godot;


// ==========================================
// BIND METHODS
// ==========================================

void Boss::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_move_speed", "value"), &Boss::set_move_speed);
    ClassDB::bind_method(D_METHOD("get_move_speed"), &Boss::get_move_speed);

    ClassDB::bind_method(D_METHOD("set_max_health", "value"), &Boss::set_max_health);

    ClassDB::bind_method(D_METHOD("set_attack_damage", "value"), &Boss::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &Boss::get_attack_damage);

    ClassDB::bind_method(D_METHOD("set_attack_stun_scale", "value"), &Boss::set_attack_stun_scale);
    ClassDB::bind_method(D_METHOD("get_attack_stun_scale"), &Boss::get_attack_stun_scale);

    ClassDB::bind_method(D_METHOD("set_attack_knockback", "value"), &Boss::set_attack_knockback);
    ClassDB::bind_method(D_METHOD("get_attack_knockback"), &Boss::get_attack_knockback);

    ClassDB::bind_method(D_METHOD("set_attack_cooldown", "value"), &Boss::set_attack_cooldown);
    ClassDB::bind_method(D_METHOD("get_attack_cooldown"), &Boss::get_attack_cooldown);

    ClassDB::bind_method(D_METHOD("set_damage_negation", "value"), &Boss::set_damage_negation);
    ClassDB::bind_method(D_METHOD("get_damage_negation"), &Boss::get_damage_negation);

    ClassDB::bind_method(D_METHOD("set_stun_negation", "value"), &Boss::set_stun_negation);
    ClassDB::bind_method(D_METHOD("get_stun_negation"), &Boss::get_stun_negation);

    ClassDB::bind_method(D_METHOD("set_knockback_negation", "value"), &Boss::set_knockback_negation);
    ClassDB::bind_method(D_METHOD("get_knockback_negation"), &Boss::get_knockback_negation);

    ClassDB::bind_method(D_METHOD("set_close_range", "value"), &Boss::set_close_range);
    ClassDB::bind_method(D_METHOD("get_close_range"), &Boss::get_close_range);

    ClassDB::bind_method(D_METHOD("set_mid_range", "value"), &Boss::set_mid_range);
    ClassDB::bind_method(D_METHOD("get_mid_range"), &Boss::get_mid_range);

    ClassDB::bind_method(D_METHOD("set_far_range", "value"), &Boss::set_far_range);
    ClassDB::bind_method(D_METHOD("get_far_range"), &Boss::get_far_range);

    ClassDB::bind_method(D_METHOD("set_skill1_damage", "value"), &Boss::set_skill1_damage);
    ClassDB::bind_method(D_METHOD("get_skill1_damage"), &Boss::get_skill1_damage);

    ClassDB::bind_method(D_METHOD("set_skill1_stun_scale", "value"), &Boss::set_skill1_stun_scale);
    ClassDB::bind_method(D_METHOD("get_skill1_stun_scale"), &Boss::get_skill1_stun_scale);

    ClassDB::bind_method(D_METHOD("set_skill1_knockback_multiplier", "value"), &Boss::set_skill1_knockback_multiplier);
    ClassDB::bind_method(D_METHOD("get_skill1_knockback_multiplier"), &Boss::get_skill1_knockback_multiplier);

    ClassDB::bind_method(D_METHOD("set_skill2_damage", "value"), &Boss::set_skill2_damage);
    ClassDB::bind_method(D_METHOD("get_skill2_damage"), &Boss::get_skill2_damage);

    ClassDB::bind_method(D_METHOD("set_skill2_stun_scale", "value"), &Boss::set_skill2_stun_scale);
    ClassDB::bind_method(D_METHOD("get_skill2_stun_scale"), &Boss::get_skill2_stun_scale);

    ClassDB::bind_method(D_METHOD("set_skill2_knockback_multiplier", "value"), &Boss::set_skill2_knockback_multiplier);
    ClassDB::bind_method(D_METHOD("get_skill2_knockback_multiplier"), &Boss::get_skill2_knockback_multiplier);

    ClassDB::bind_method(D_METHOD("set_skill3_damage", "value"), &Boss::set_skill3_damage);
    ClassDB::bind_method(D_METHOD("get_skill3_damage"), &Boss::get_skill3_damage);

    ClassDB::bind_method(D_METHOD("set_skill3_stun_scale", "value"), &Boss::set_skill3_stun_scale);
    ClassDB::bind_method(D_METHOD("get_skill3_stun_scale"), &Boss::get_skill3_stun_scale);

    ClassDB::bind_method(D_METHOD("set_skill3_knockback_multiplier", "value"), &Boss::set_skill3_knockback_multiplier);
    ClassDB::bind_method(D_METHOD("get_skill3_knockback_multiplier"), &Boss::get_skill3_knockback_multiplier);

    ClassDB::bind_method(D_METHOD("set_skill4_damage", "value"), &Boss::set_skill4_damage);
    ClassDB::bind_method(D_METHOD("get_skill4_damage"), &Boss::get_skill4_damage);

    ClassDB::bind_method(D_METHOD("set_skill4_stun_scale", "value"), &Boss::set_skill4_stun_scale);
    ClassDB::bind_method(D_METHOD("get_skill4_stun_scale"), &Boss::get_skill4_stun_scale);

    ClassDB::bind_method(D_METHOD("set_skill4_knockback_multiplier", "value"), &Boss::set_skill4_knockback_multiplier);
    ClassDB::bind_method(D_METHOD("get_skill4_knockback_multiplier"), &Boss::get_skill4_knockback_multiplier);

    ClassDB::bind_method(D_METHOD("set_skill5_damage", "value"), &Boss::set_skill5_damage);
    ClassDB::bind_method(D_METHOD("get_skill5_damage"), &Boss::get_skill5_damage);

    ClassDB::bind_method(D_METHOD("set_skill5_stun_scale", "value"), &Boss::set_skill5_stun_scale);
    ClassDB::bind_method(D_METHOD("get_skill5_stun_scale"), &Boss::get_skill5_stun_scale);

    ClassDB::bind_method(D_METHOD("set_skill5_knockback_multiplier", "value"), &Boss::set_skill5_knockback_multiplier);
    ClassDB::bind_method(D_METHOD("get_skill5_knockback_multiplier"), &Boss::get_skill5_knockback_multiplier);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_speed", PROPERTY_HINT_RANGE, "0,1000,1"), "set_move_speed", "get_move_speed");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_RANGE, "1,10000,1"), "set_max_health", "get_max_health");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_damage", "get_attack_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_attack_stun_scale", "get_attack_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_knockback", "get_attack_knockback");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_cooldown", PROPERTY_HINT_RANGE, "0.1,10,0.1"), "set_attack_cooldown", "get_attack_cooldown");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "skill1_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_skill1_damage", "get_skill1_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill1_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_skill1_stun_scale", "get_skill1_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill1_knockback_multiplier", PROPERTY_HINT_RANGE, "0,10,0.05"), "set_skill1_knockback_multiplier", "get_skill1_knockback_multiplier");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "skill2_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_skill2_damage", "get_skill2_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill2_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_skill2_stun_scale", "get_skill2_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill2_knockback_multiplier", PROPERTY_HINT_RANGE, "0,10,0.05"), "set_skill2_knockback_multiplier", "get_skill2_knockback_multiplier");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "skill3_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_skill3_damage", "get_skill3_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill3_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_skill3_stun_scale", "get_skill3_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill3_knockback_multiplier", PROPERTY_HINT_RANGE, "0,10,0.05"), "set_skill3_knockback_multiplier", "get_skill3_knockback_multiplier");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "skill4_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_skill4_damage", "get_skill4_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill4_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_skill4_stun_scale", "get_skill4_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill4_knockback_multiplier", PROPERTY_HINT_RANGE, "0,10,0.05"), "set_skill4_knockback_multiplier", "get_skill4_knockback_multiplier");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "skill5_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_skill5_damage", "get_skill5_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill5_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_skill5_stun_scale", "get_skill5_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "skill5_knockback_multiplier", PROPERTY_HINT_RANGE, "0,10,0.05"), "set_skill5_knockback_multiplier", "get_skill5_knockback_multiplier");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damage_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_damage_negation", "get_damage_negation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stun_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_stun_negation", "get_stun_negation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "knockback_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_knockback_negation", "get_knockback_negation");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "close_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_close_range", "get_close_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mid_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_mid_range", "get_mid_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "far_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_far_range", "get_far_range");
}


// ==========================================
// CONSTRUCTOR
// ==========================================

Boss::Boss() {
    max_health = 500;
    health = max_health;
    move_speed = 80.0f;
}

Boss::~Boss() {
}


// ==========================================
// READY
// ==========================================

void Boss::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) return;

    health = max_health;

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));

    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));

    skill1_hitbox1 = get_node<Area2D>(NodePath("hitbox_skill1/hitbox1"));
    skill1_hitbox2 = get_node<Area2D>(NodePath("hitbox_skill1/hitbox2"));

    skill2_hitbox = get_node<Area2D>(NodePath("hitbox_skill2/Area2D"));
    skill3_hitbox = get_node<Area2D>(NodePath("hitbox_skill3/Area2D"));

    skill4_hitbox1 = get_node<Area2D>(NodePath("hitbox_skill4/hitbox1"));
    skill4_hitbox2 = get_node<Area2D>(NodePath("hitbox_skill4/hitbox2"));

    skill5_hitbox1 = get_node<Area2D>(NodePath("hitbox_skill5/hitbox1"));
    skill5_hitbox2 = get_node<Area2D>(NodePath("hitbox_skill5/hitbox2"));

    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));

    parry_effect = get_node<AnimatedSprite2D>(NodePath("effects/parry"));
    parry_sound = get_node<Node>(NodePath("effects/ParrySound"));

    attack_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/attack"));
    skill1_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/skill1"));
    skill2_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/skill2"));
    skill3_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/skill3"));
    skill4_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/skill4"));
    skill5_sound = get_node<AudioStreamPlayer2D>(NodePath("effects/attacksound/skill5"));

    body_collision = get_node<CollisionShape2D>(NodePath("CollisionShape2D"));

    // ==========================================
    // SAVE DEFAULT TRANSFORMS
    // ==========================================

    animated_sprite_default_position = animated_sprite->get_position();
    animated_sprite_default_scale = animated_sprite->get_scale();

    parry_effect_default_position = parry_effect->get_position();
    parry_effect_default_scale = parry_effect->get_scale();

    hitbox_default_position = hitbox->get_position();
    hitbox_default_scale = hitbox->get_scale();

    skill1_hitbox1_default_position = skill1_hitbox1->get_position();
    skill1_hitbox1_default_scale = skill1_hitbox1->get_scale();

    skill1_hitbox2_default_position = skill1_hitbox2->get_position();
    skill1_hitbox2_default_scale = skill1_hitbox2->get_scale();

    skill2_hitbox_default_position = skill2_hitbox->get_position();
    skill2_hitbox_default_scale = skill2_hitbox->get_scale();

    skill3_hitbox_default_position = skill3_hitbox->get_position();
    skill3_hitbox_default_scale = skill3_hitbox->get_scale();

    skill4_hitbox1_default_position = skill4_hitbox1->get_position();
    skill4_hitbox1_default_scale = skill4_hitbox1->get_scale();

    skill4_hitbox2_default_position = skill4_hitbox2->get_position();
    skill4_hitbox2_default_scale = skill4_hitbox2->get_scale();

    skill5_hitbox1_default_position = skill5_hitbox1->get_position();
    skill5_hitbox1_default_scale = skill5_hitbox1->get_scale();

    skill5_hitbox2_default_position = skill5_hitbox2->get_position();
    skill5_hitbox2_default_scale = skill5_hitbox2->get_scale();

    animated_sprite->set_flip_h(false);

    parry_effect->stop();
    parry_effect->set_visible(false);

    // ==========================================
    // HITBOX SETUP
    // ==========================================

    skill1_hitbox1->set_collision_layer(0);
    skill1_hitbox2->set_collision_layer(0);
    skill2_hitbox->set_collision_layer(0);
    skill3_hitbox->set_collision_layer(0);
    skill4_hitbox1->set_collision_layer(0);
    skill4_hitbox2->set_collision_layer(0);
    skill5_hitbox1->set_collision_layer(0);
    skill5_hitbox2->set_collision_layer(0);

    skill1_hitbox1->set_collision_mask(hitbox->get_collision_mask());
    skill1_hitbox2->set_collision_mask(hitbox->get_collision_mask());
    skill2_hitbox->set_collision_mask(hitbox->get_collision_mask());
    skill3_hitbox->set_collision_mask(hitbox->get_collision_mask());
    skill4_hitbox1->set_collision_mask(hitbox->get_collision_mask());
    skill4_hitbox2->set_collision_mask(hitbox->get_collision_mask());
    skill5_hitbox1->set_collision_mask(hitbox->get_collision_mask());
    skill5_hitbox2->set_collision_mask(hitbox->get_collision_mask());

    disable_attack_hitboxes();

    // ==========================================
    // START
    // ==========================================

    find_target();

    random_manager = get_node<Node>(NodePath("/root/RandomManager"));

    attack_cooldown_timer = 0.0f;

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}


// ==========================================
// FIND TARGET
// ==========================================

void Boss::find_target() {
    target = Object::cast_to<Actor>(get_tree()->get_first_node_in_group("player"));
}


// ==========================================
// DECIDE ACTION
// ==========================================

void Boss::decide_action() {
    if (target == nullptr || target->get_health() <= 0) {
        combat_state = CombatState::IDLE;
        moving_from_range = 0;
        return;
    }

    if (combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) return;

    Vector2 player_position = target->get_global_position();
    Vector2 boss_position = get_global_position();
    Vector2 offset = player_position - boss_position;

    float total_distance = offset.length();

    update_facing(offset.x);


    // ==========================================
    // ATTACK COOLDOWN
    // ==========================================

    if (attack_cooldown_timer > 0.0f) {
        combat_state = CombatState::IDLE;
        moving_from_range = 0;
        return;
    }


    // ==========================================
    // OUTSIDE DETECTION
    // ==========================================

    if (total_distance > far_range) {
        combat_state = CombatState::IDLE;
        moving_from_range = 0;
        return;
    }


    // ==========================================
    // CLOSE CHECK
    // ==========================================

    bool blocked = get_slide_collision_count() > 0;
    bool close_x = Math::abs(offset.x) <= close_range;
    bool aligned_y = Math::abs(offset.y) <= attack_align_tolerance;
    bool close_ready = close_x && (aligned_y || blocked);


    // ==========================================
    // KEEP MOVING FROM FAR
    // ==========================================

    if (moving_from_range == 1) {
        if (total_distance > mid_range) {
            combat_state = CombatState::MOVING;
            return;
        }

        moving_from_range = 0;
    }


    // ==========================================
    // KEEP MOVING FROM MID
    // ==========================================

    if (moving_from_range == 2) {
        if (!close_ready) {
            combat_state = CombatState::MOVING;
            return;
        }

        moving_from_range = 0;
    }


    // ==========================================
    // CLOSE RANGE
    // Attack 25%
    // Skill 1 5%
    // Skill 2 30%
    // Skill 3 20%
    // Skill 4 20%
    // ==========================================

    if (close_ready) {
        int roll = static_cast<int>(random_manager->call("random_int", 0, 999));

        if (roll < 250) attack();
        else if (roll < 300) skill_1();
        else if (roll < 600) skill_2();
        else if (roll < 800) skill_3();
        else skill_4();

        return;
    }


    // Nếu đã vào vùng close theo khoảng cách nhưng chưa align Y,
    // chỉ tiếp tục di chuyển để align thay vì dùng bảng MID.
    if (total_distance <= close_range) {
        combat_state = CombatState::MOVING;
        moving_from_range = 2;
        return;
    }


    // ==========================================
    // MID RANGE
    // Skill 1 18%
    // Skill 3 18%
    // Skill 4 18%
    // Skill 5 10%
    // Move 36%
    // ==========================================

    if (total_distance <= mid_range) {
        int roll = static_cast<int>(random_manager->call("random_int", 0, 999));

        if (roll < 180) {
            skill_1();
        }
        else if (roll < 360) {
            skill_3();
        }
        else if (roll < 540) {
            skill_4();
        }
        else if (roll < 640) {
            skill_5();
        }
        else {
            combat_state = CombatState::MOVING;
            moving_from_range = 2;
        }

        return;
    }


    // ==========================================
    // FAR RANGE
    // Skill 5 30%
    // Move 70%
    // ==========================================

    int roll = static_cast<int>(random_manager->call("random_int", 0, 999));

    if (roll < 300) {
        skill_5();
    }
    else {
        combat_state = CombatState::MOVING;
        moving_from_range = 1;
    }
}


// ==========================================
// TRY PARRY
// ==========================================

void Boss::try_parry() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (is_parrying || combat_state == CombatState::ATTACKING) return;

    Vector2 offset = target->get_global_position() - get_global_position();

    bool blocked = get_slide_collision_count() > 0;
    bool close_x = Math::abs(offset.x) <= close_range;
    bool aligned_y = Math::abs(offset.y) <= attack_align_tolerance;
    bool close_ready = close_x && (aligned_y || blocked);

    if (!close_ready) return;

    int roll = static_cast<int>(random_manager->call("random_int", 0, 99));

    if (roll >= parry_chance) return;

    parry();
}


// ==========================================
// PARRY
// ==========================================

void Boss::parry() {
    is_parrying = true;
    moving_from_range = 0;

    attack_cooldown_timer = 0.0f;

    disable_attack_hitboxes();

    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));

    update_facing(target->get_global_position().x - get_global_position().x);

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("parry");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    parry_effect->set_visible(true);
    parry_effect->stop();
    parry_effect->set_speed_scale(1.0f);
    parry_effect->play("parry");
    parry_effect->set_frame_and_progress(0, 0.0f);

    parry_sound->call("stop");
    parry_sound->call("play");
}


// ==========================================
// TAKE DAMAGE
// ==========================================

void Boss::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0 || is_parrying) return;

    int final_damage = static_cast<int>(damage * (1.0f - damage_negation) + 0.5f);

    Actor::take_damage(final_damage, stun_scale, knockback, attacker);

    if (health <= 0) die();

}

void Boss::die(bool play_sound) {
    disable_attack_hitboxes();
    hurtbox->set_deferred("monitorable", false);
    body_collision->set_deferred("disabled", true);

    is_parrying = false;
    parry_effect->stop();
    parry_effect->hide();

    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));
    set_physics_process(false);

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("die");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}


// ==========================================
// NORMAL ATTACK
// ==========================================

void Boss::attack() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    attack_has_hit = false;

    disable_attack_hitboxes();

    hitbox->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("attack");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (attack_sound != nullptr) attack_sound->play();
}


// ==========================================
// SKILL 1
// ==========================================

void Boss::skill_1() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    skill1_hitbox1_first_hit = false;
    skill1_hitbox1_second_hit = false;
    skill1_hitbox2_first_hit = false;
    skill1_hitbox2_second_hit = false;

    disable_attack_hitboxes();

    skill1_hitbox1->set_monitoring(true);
    skill1_hitbox2->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("skill1");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (skill1_sound != nullptr) skill1_sound->play();
}


// ==========================================
// SKILL 2
// ==========================================

void Boss::skill_2() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    skill2_first_hit = false;
    skill2_second_hit = false;

    disable_attack_hitboxes();

    skill2_hitbox->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("skill2");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (skill2_sound != nullptr) skill2_sound->play();
}


// ==========================================
// SKILL 3
// ==========================================

void Boss::skill_3() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    skill3_first_hit = false;
    skill3_second_hit = false;

    disable_attack_hitboxes();

    skill3_hitbox->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("skill3");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (skill3_sound != nullptr) skill3_sound->play();
}


// ==========================================
// SKILL 4
// ==========================================

void Boss::skill_4() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    skill4_first_hit = false;
    skill4_second_hit = false;

    disable_attack_hitboxes();

    skill4_hitbox1->set_monitoring(true);
    skill4_hitbox2->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("skill4");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (skill4_sound != nullptr) skill4_sound->play();
}


// ==========================================
// SKILL 5
// ==========================================

void Boss::skill_5() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (attack_cooldown_timer > 0.0f) return;

    combat_state = CombatState::ATTACKING;

    skill5_first_hit = false;
    skill5_second_hit = false;

    disable_attack_hitboxes();

    skill5_hitbox1->set_monitoring(true);
    skill5_hitbox2->set_monitoring(true);

    update_facing(target->get_global_position().x - get_global_position().x);

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("skill5");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    if (skill5_sound != nullptr) skill5_sound->play();
}


// ==========================================
// UPDATE ATTACK
// ==========================================

void Boss::update_attack() {
    int frame = animated_sprite->get_frame();
    StringName animation = animated_sprite->get_animation();

    // ==========================================
    // NORMAL ATTACK
    // ==========================================

    if (animated_sprite->is_playing() && animation == StringName("attack")) {
        if (frame >= 4 && frame <= 5) {
            apply_attack_hit(hitbox, attack_has_hit, attack_damage, attack_stun_scale, attack_knockback);
        }
    }

    // ==========================================
    // SKILL 1
    // ==========================================

    else if (animated_sprite->is_playing() && animation == StringName("skill1")) {
        float skill1_knockback = attack_knockback * skill1_knockback_multiplier;

        if (frame >= 5 && frame <= 6) {
            apply_attack_hit(skill1_hitbox1, skill1_hitbox1_first_hit, skill1_damage, skill1_stun_scale, skill1_knockback);
        }

        if (frame >= 11 && frame <= 13) {
            apply_attack_hit(skill1_hitbox2, skill1_hitbox2_first_hit, skill1_damage, skill1_stun_scale, skill1_knockback);
        }

        if (frame >= 15 && frame <= 19) {
            apply_attack_hit(skill1_hitbox2, skill1_hitbox2_second_hit, skill1_damage, skill1_stun_scale, skill1_knockback);
        }

        if (frame >= 26 && frame <= 27) {
            apply_attack_hit(skill1_hitbox1, skill1_hitbox1_second_hit, skill1_damage, skill1_stun_scale, skill1_knockback);
        }
    }

    // ==========================================
    // SKILL 2
    // ==========================================

    else if (animated_sprite->is_playing() && animation == StringName("skill2")) {
        float skill2_knockback = attack_knockback * skill2_knockback_multiplier;

        if (frame >= 4 && frame <= 5) {
            apply_attack_hit(skill2_hitbox, skill2_first_hit, skill2_damage, skill2_stun_scale, skill2_knockback);
        }

        if (frame >= 9 && frame <= 10) {
            apply_attack_hit(skill2_hitbox, skill2_second_hit, skill2_damage, skill2_stun_scale, skill2_knockback);
        }
    }

    // ==========================================
    // SKILL 3
    // ==========================================

    else if (animated_sprite->is_playing() && animation == StringName("skill3")) {
        float skill3_knockback = attack_knockback * skill3_knockback_multiplier;

        if (frame >= 6 && frame <= 7) {
            apply_attack_hit(skill3_hitbox, skill3_first_hit, skill3_damage, skill3_stun_scale, skill3_knockback);
        }

        if (frame >= 14 && frame <= 15) {
            apply_attack_hit(skill3_hitbox, skill3_second_hit, skill3_damage, skill3_stun_scale, skill3_knockback);
        }
    }

    // ==========================================
    // SKILL 4
    // ==========================================

    else if (animated_sprite->is_playing() && animation == StringName("skill4")) {
        float skill4_knockback = attack_knockback * skill4_knockback_multiplier;

        if (frame >= 8 && frame <= 10) {
            apply_attack_hit(skill4_hitbox1, skill4_first_hit, skill4_damage, skill4_stun_scale, skill4_knockback);
        }

        if (frame >= 21 && frame <= 22) {
            apply_attack_hit(skill4_hitbox2, skill4_second_hit, skill4_damage, skill4_stun_scale, skill4_knockback);
        }
    }

    // ==========================================
    // SKILL 5
    // ==========================================

    else if (animated_sprite->is_playing() && animation == StringName("skill5")) {
        float skill5_knockback = attack_knockback * skill5_knockback_multiplier;

        if (frame >= 4 && frame <= 6) {
            apply_attack_hit(skill5_hitbox1, skill5_first_hit, skill5_damage, skill5_stun_scale, skill5_knockback);
        }

        if (frame >= 10 && frame <= 11) {
            apply_attack_hit(skill5_hitbox2, skill5_second_hit, skill5_damage, skill5_stun_scale, skill5_knockback);
        }
    }

    // ==========================================
    // FINISH ATTACK
    // ==========================================

    if (!animated_sprite->is_playing()) {
        disable_attack_hitboxes();

        combat_state = CombatState::IDLE;
        attack_cooldown_timer = attack_cooldown;

        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("idle");
    }
}


// ==========================================
// APPLY ATTACK HIT
// ==========================================

void Boss::apply_attack_hit(Area2D *active_hitbox, bool &has_hit, int damage, float stun_scale, float knockback) {
    if (has_hit || target == nullptr || target->get_health() <= 0) return;

    TypedArray<Area2D> areas = active_hitbox->get_overlapping_areas();

    for (int i = 0; i < areas.size(); i++) {
        for (Node *node = Object::cast_to<Area2D>(areas[i]); node != nullptr; node = node->get_parent()) {
            if (node != target) continue;

            has_hit = true;

            target->take_damage(damage, stun_scale, knockback, this);

            return;
        }
    }
}


// ==========================================
// DISABLE ATTACK HITBOXES
// ==========================================

void Boss::disable_attack_hitboxes() {
    hitbox->set_monitoring(false);

    skill1_hitbox1->set_monitoring(false);
    skill1_hitbox2->set_monitoring(false);

    skill2_hitbox->set_monitoring(false);
    skill3_hitbox->set_monitoring(false);

    skill4_hitbox1->set_monitoring(false);
    skill4_hitbox2->set_monitoring(false);

    skill5_hitbox1->set_monitoring(false);
    skill5_hitbox2->set_monitoring(false);
}


// ==========================================
// UPDATE FACING
// ==========================================

void Boss::update_facing(float direction_x) {
    if (direction_x == 0.0f) return;

    bool face_left = direction_x < 0.0f;

    // ==========================================
    // SPRITE
    // ==========================================

    Vector2 sprite_position = animated_sprite_default_position;
    Vector2 sprite_scale = animated_sprite_default_scale;

    if (face_left) {
        sprite_position.x = -animated_sprite_default_position.x;
        sprite_scale.x = -Math::abs(animated_sprite_default_scale.x);
    }
    else {
        sprite_position.x = animated_sprite_default_position.x;
        sprite_scale.x = Math::abs(animated_sprite_default_scale.x);
    }

    animated_sprite->set_position(sprite_position);
    animated_sprite->set_scale(sprite_scale);
    animated_sprite->set_flip_h(false);

    // ==========================================
    // PARRY EFFECT
    // ==========================================

    Vector2 parry_position = parry_effect_default_position;
    Vector2 parry_scale = parry_effect_default_scale;

    if (face_left) {
        parry_position.x = -parry_effect_default_position.x;
        parry_scale.x = -Math::abs(parry_effect_default_scale.x);
    }
    else {
        parry_position.x = parry_effect_default_position.x;
        parry_scale.x = Math::abs(parry_effect_default_scale.x);
    }

    parry_effect->set_position(parry_position);
    parry_effect->set_scale(parry_scale);
    parry_effect->set_flip_h(false);

    // ==========================================
    // NORMAL HITBOX
    // ==========================================

    Vector2 hitbox_position = hitbox_default_position;
    Vector2 hitbox_scale = hitbox_default_scale;

    if (face_left) {
        hitbox_position.x = -hitbox_default_position.x;
        hitbox_scale.x = -Math::abs(hitbox_default_scale.x);
    }
    else {
        hitbox_position.x = hitbox_default_position.x;
        hitbox_scale.x = Math::abs(hitbox_default_scale.x);
    }

    hitbox->set_position(hitbox_position);
    hitbox->set_scale(hitbox_scale);

    // ==========================================
    // SKILL 1 HITBOX 1
    // ==========================================

    Vector2 skill1_position1 = skill1_hitbox1_default_position;
    Vector2 skill1_scale1 = skill1_hitbox1_default_scale;

    if (face_left) {
        skill1_position1.x = -skill1_hitbox1_default_position.x;
        skill1_scale1.x = -Math::abs(skill1_hitbox1_default_scale.x);
    }
    else {
        skill1_position1.x = skill1_hitbox1_default_position.x;
        skill1_scale1.x = Math::abs(skill1_hitbox1_default_scale.x);
    }

    skill1_hitbox1->set_position(skill1_position1);
    skill1_hitbox1->set_scale(skill1_scale1);

    // ==========================================
    // SKILL 1 HITBOX 2
    // ==========================================

    Vector2 skill1_position2 = skill1_hitbox2_default_position;
    Vector2 skill1_scale2 = skill1_hitbox2_default_scale;

    if (face_left) {
        skill1_position2.x = -skill1_hitbox2_default_position.x;
        skill1_scale2.x = -Math::abs(skill1_hitbox2_default_scale.x);
    }
    else {
        skill1_position2.x = skill1_hitbox2_default_position.x;
        skill1_scale2.x = Math::abs(skill1_hitbox2_default_scale.x);
    }

    skill1_hitbox2->set_position(skill1_position2);
    skill1_hitbox2->set_scale(skill1_scale2);

    // ==========================================
    // SKILL 2 HITBOX
    // ==========================================

    Vector2 skill2_position = skill2_hitbox_default_position;
    Vector2 skill2_scale = skill2_hitbox_default_scale;

    if (face_left) {
        skill2_position.x = -skill2_hitbox_default_position.x;
        skill2_scale.x = -Math::abs(skill2_hitbox_default_scale.x);
    }
    else {
        skill2_position.x = skill2_hitbox_default_position.x;
        skill2_scale.x = Math::abs(skill2_hitbox_default_scale.x);
    }

    skill2_hitbox->set_position(skill2_position);
    skill2_hitbox->set_scale(skill2_scale);

    // ==========================================
    // SKILL 3 HITBOX
    // ==========================================

    Vector2 skill3_position = skill3_hitbox_default_position;
    Vector2 skill3_scale = skill3_hitbox_default_scale;

    if (face_left) {
        skill3_position.x = -skill3_hitbox_default_position.x;
        skill3_scale.x = -Math::abs(skill3_hitbox_default_scale.x);
    }
    else {
        skill3_position.x = skill3_hitbox_default_position.x;
        skill3_scale.x = Math::abs(skill3_hitbox_default_scale.x);
    }

    skill3_hitbox->set_position(skill3_position);
    skill3_hitbox->set_scale(skill3_scale);

    // ==========================================
    // SKILL 4 HITBOX 1
    // ==========================================

    Vector2 skill4_position1 = skill4_hitbox1_default_position;
    Vector2 skill4_scale1 = skill4_hitbox1_default_scale;

    if (face_left) {
        skill4_position1.x = -skill4_hitbox1_default_position.x;
        skill4_scale1.x = -Math::abs(skill4_hitbox1_default_scale.x);
    }
    else {
        skill4_position1.x = skill4_hitbox1_default_position.x;
        skill4_scale1.x = Math::abs(skill4_hitbox1_default_scale.x);
    }

    skill4_hitbox1->set_position(skill4_position1);
    skill4_hitbox1->set_scale(skill4_scale1);

    // ==========================================
    // SKILL 4 HITBOX 2
    // ==========================================

    Vector2 skill4_position2 = skill4_hitbox2_default_position;
    Vector2 skill4_scale2 = skill4_hitbox2_default_scale;

    if (face_left) {
        skill4_position2.x = -skill4_hitbox2_default_position.x;
        skill4_scale2.x = -Math::abs(skill4_hitbox2_default_scale.x);
    }
    else {
        skill4_position2.x = skill4_hitbox2_default_position.x;
        skill4_scale2.x = Math::abs(skill4_hitbox2_default_scale.x);
    }

    skill4_hitbox2->set_position(skill4_position2);
    skill4_hitbox2->set_scale(skill4_scale2);

    // ==========================================
    // SKILL 5 HITBOX 1
    // ==========================================

    Vector2 skill5_position1 = skill5_hitbox1_default_position;
    Vector2 skill5_scale1 = skill5_hitbox1_default_scale;

    if (face_left) {
        skill5_position1.x = -skill5_hitbox1_default_position.x;
        skill5_scale1.x = -Math::abs(skill5_hitbox1_default_scale.x);
    }
    else {
        skill5_position1.x = skill5_hitbox1_default_position.x;
        skill5_scale1.x = Math::abs(skill5_hitbox1_default_scale.x);
    }

    skill5_hitbox1->set_position(skill5_position1);
    skill5_hitbox1->set_scale(skill5_scale1);

    // ==========================================
    // SKILL 5 HITBOX 2
    // ==========================================

    Vector2 skill5_position2 = skill5_hitbox2_default_position;
    Vector2 skill5_scale2 = skill5_hitbox2_default_scale;

    if (face_left) {
        skill5_position2.x = -skill5_hitbox2_default_position.x;
        skill5_scale2.x = -Math::abs(skill5_hitbox2_default_scale.x);
    }
    else {
        skill5_position2.x = skill5_hitbox2_default_position.x;
        skill5_scale2.x = Math::abs(skill5_hitbox2_default_scale.x);
    }

    skill5_hitbox2->set_position(skill5_position2);
    skill5_hitbox2->set_scale(skill5_scale2);
}


// ==========================================
// PHYSICS PROCESS
// ==========================================

void Boss::_physics_process(double delta) {
    if (delta <= 0.0) return;
    if (Engine::get_singleton()->is_editor_hint()) return;
    if (health <= 0) return;

    Input *input = Input::get_singleton();

    // ==========================================
    // PARRY INPUT
    // ==========================================

    if (input->is_action_just_pressed("attack")) try_parry();

    // ==========================================
    // PARRYING
    // ==========================================

    if (is_parrying) {
        disable_attack_hitboxes();
        set_velocity(Vector2(0, 0));
        move_and_slide();

        if (!animated_sprite->is_playing()) {
            is_parrying = false;

            parry_effect->stop();
            parry_effect->set_visible(false);

            combat_state = CombatState::IDLE;
            moving_from_range = 0;
        }

        return;
    }

    // ==========================================
    // ATTACK COOLDOWN
    // ==========================================

    if (attack_cooldown_timer > 0.0f) {
        attack_cooldown_timer -= static_cast<float>(delta);

        if (attack_cooldown_timer < 0.0f) attack_cooldown_timer = 0.0f;
    }

    Vector2 movement_velocity = Vector2(0, 0);

    // ==========================================
    // AI DECISION
    // ==========================================

    if (combat_state != CombatState::ATTACKING && combat_state != CombatState::STUNNED) decide_action();

    // ==========================================
    // IDLE
    // ==========================================

    if (combat_state == CombatState::IDLE) {
        disable_attack_hitboxes();

        if (animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");
        }
    }

    // ==========================================
    // MOVING
    // ==========================================

    else if (combat_state == CombatState::MOVING) {
        disable_attack_hitboxes();

        if (target != nullptr) {
            Vector2 player_position = target->get_global_position();
            Vector2 boss_position = get_global_position();

            Vector2 offset = player_position - boss_position;

            float x_distance = std::max(
                0.0f,
                static_cast<float>(Math::abs(offset.x)) - close_range * 0.9f
            );

            Vector2 approach = Vector2(
                offset.x >= 0.0f ? x_distance : -x_distance,
                offset.y
            );

            if (Math::abs(offset.x) <= close_range) approach.x = 0.0f;

            float speed = std::min(
                move_speed,
                static_cast<float>(approach.length() / delta)
            );

            movement_velocity = approach.normalized() * speed;

            update_facing(offset.x);

            if (animated_sprite->get_animation() != StringName("run")) {
                animated_sprite->set_speed_scale(1.0f);
                animated_sprite->play("run");
            }
        }
    }

    // ==========================================
    // ATTACKING
    // ==========================================

    else if (combat_state == CombatState::ATTACKING) {
        update_attack();
    }

    // ==========================================
    // KNOCKBACK
    // ==========================================

    if (knockback_velocity.length() > 0.0f) {
        knockback_velocity = knockback_velocity.move_toward(
            Vector2(0, 0),
            500.0f * static_cast<float>(delta)
        );

        if (knockback_velocity.length() < 1.0f) knockback_velocity = Vector2(0, 0);
    }

    // ==========================================
    // FINAL MOVEMENT
    // ==========================================

    set_velocity(movement_velocity + knockback_velocity);
    move_and_slide();
}


// ==========================================
// SETTERS / GETTERS
// ==========================================

void Boss::set_move_speed(float value) { move_speed = value; }
float Boss::get_move_speed() const { return move_speed; }

void Boss::set_max_health(int value) { max_health = value; }

void Boss::set_attack_damage(int value) { attack_damage = value; }
int Boss::get_attack_damage() const { return attack_damage; }

void Boss::set_attack_stun_scale(float value) { attack_stun_scale = value; }
float Boss::get_attack_stun_scale() const { return attack_stun_scale; }

void Boss::set_attack_knockback(float value) { attack_knockback = value; }
float Boss::get_attack_knockback() const { return attack_knockback; }

void Boss::set_attack_cooldown(float value) { attack_cooldown = value; }
float Boss::get_attack_cooldown() const { return attack_cooldown; }

void Boss::set_damage_negation(float value) { damage_negation = value; }
float Boss::get_damage_negation() const { return damage_negation; }

void Boss::set_stun_negation(float value) { stun_negation = value; }
float Boss::get_stun_negation() const { return stun_negation; }

void Boss::set_knockback_negation(float value) { knockback_negation = value; }
float Boss::get_knockback_negation() const { return knockback_negation; }

void Boss::set_close_range(float value) { close_range = value; }
float Boss::get_close_range() const { return close_range; }

void Boss::set_mid_range(float value) { mid_range = value; }
float Boss::get_mid_range() const { return mid_range; }

void Boss::set_far_range(float value) { far_range = value; }
float Boss::get_far_range() const { return far_range; }


// ==========================================
// SKILL 1 SETTERS / GETTERS
// ==========================================

void Boss::set_skill1_damage(int value) { skill1_damage = value; }
int Boss::get_skill1_damage() const { return skill1_damage; }

void Boss::set_skill1_stun_scale(float value) { skill1_stun_scale = value; }
float Boss::get_skill1_stun_scale() const { return skill1_stun_scale; }

void Boss::set_skill1_knockback_multiplier(float value) { skill1_knockback_multiplier = value; }
float Boss::get_skill1_knockback_multiplier() const { return skill1_knockback_multiplier; }


// ==========================================
// SKILL 2 SETTERS / GETTERS
// ==========================================

void Boss::set_skill2_damage(int value) { skill2_damage = value; }
int Boss::get_skill2_damage() const { return skill2_damage; }

void Boss::set_skill2_stun_scale(float value) { skill2_stun_scale = value; }
float Boss::get_skill2_stun_scale() const { return skill2_stun_scale; }

void Boss::set_skill2_knockback_multiplier(float value) { skill2_knockback_multiplier = value; }
float Boss::get_skill2_knockback_multiplier() const { return skill2_knockback_multiplier; }

// ==========================================
// SKILL 3 SETTERS / GETTERS
// ==========================================

void Boss::set_skill3_damage(int value) { skill3_damage = value; }
int Boss::get_skill3_damage() const { return skill3_damage; }

void Boss::set_skill3_stun_scale(float value) { skill3_stun_scale = value; }
float Boss::get_skill3_stun_scale() const { return skill3_stun_scale; }

void Boss::set_skill3_knockback_multiplier(float value) { skill3_knockback_multiplier = value; }
float Boss::get_skill3_knockback_multiplier() const { return skill3_knockback_multiplier; }




// ==========================================
// SKILL 4 SETTERS / GETTERS
// ==========================================

void Boss::set_skill4_damage(int value) { skill4_damage = value; }
int Boss::get_skill4_damage() const { return skill4_damage; }

void Boss::set_skill4_stun_scale(float value) { skill4_stun_scale = value; }
float Boss::get_skill4_stun_scale() const { return skill4_stun_scale; }

void Boss::set_skill4_knockback_multiplier(float value) { skill4_knockback_multiplier = value; }
float Boss::get_skill4_knockback_multiplier() const { return skill4_knockback_multiplier; }



// ==========================================
// SKILL 5 SETTERS / GETTERS
// ==========================================

void Boss::set_skill5_damage(int value) { skill5_damage = value; }
int Boss::get_skill5_damage() const { return skill5_damage; }

void Boss::set_skill5_stun_scale(float value) { skill5_stun_scale = value; }
float Boss::get_skill5_stun_scale() const { return skill5_stun_scale; }

void Boss::set_skill5_knockback_multiplier(float value) { skill5_knockback_multiplier = value; }
float Boss::get_skill5_knockback_multiplier() const { return skill5_knockback_multiplier; }
