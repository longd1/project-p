#include "mini_boss.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable_method_pointer.hpp>
#include <godot_cpp/core/math.hpp>
#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/shape2d.hpp>
#include <godot_cpp/classes/kinematic_collision2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// ==========================================
// BIND METHODS
// ==========================================

void MiniBoss::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_move_speed", "value"), &MiniBoss::set_move_speed);
    ClassDB::bind_method(D_METHOD("get_move_speed"), &MiniBoss::get_move_speed);

    ClassDB::bind_method(D_METHOD("set_close_range", "value"), &MiniBoss::set_close_range);
    ClassDB::bind_method(D_METHOD("get_close_range"), &MiniBoss::get_close_range);

    ClassDB::bind_method(D_METHOD("set_mid_range", "value"), &MiniBoss::set_mid_range);
    ClassDB::bind_method(D_METHOD("get_mid_range"), &MiniBoss::get_mid_range);

    ClassDB::bind_method(D_METHOD("set_far_range", "value"), &MiniBoss::set_far_range);
    ClassDB::bind_method(D_METHOD("get_far_range"), &MiniBoss::get_far_range);

    ClassDB::bind_method(D_METHOD("set_max_health", "value"), &MiniBoss::set_max_health);

    ClassDB::bind_method(D_METHOD("set_attack_damage", "value"), &MiniBoss::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &MiniBoss::get_attack_damage);

    ClassDB::bind_method(D_METHOD("set_attack_stun_scale", "value"), &MiniBoss::set_attack_stun_scale);
    ClassDB::bind_method(D_METHOD("get_attack_stun_scale"), &MiniBoss::get_attack_stun_scale);

    ClassDB::bind_method(D_METHOD("set_attack_knockback", "value"), &MiniBoss::set_attack_knockback);
    ClassDB::bind_method(D_METHOD("get_attack_knockback"), &MiniBoss::get_attack_knockback);

    ClassDB::bind_method(D_METHOD("set_attack_cooldown", "value"), &MiniBoss::set_attack_cooldown);
    ClassDB::bind_method(D_METHOD("get_attack_cooldown"), &MiniBoss::get_attack_cooldown);

    ClassDB::bind_method(D_METHOD("set_stun_negation", "value"), &MiniBoss::set_stun_negation);
    ClassDB::bind_method(D_METHOD("get_stun_negation"), &MiniBoss::get_stun_negation);

    ClassDB::bind_method(D_METHOD("set_knockback_negation", "value"), &MiniBoss::set_knockback_negation);
    ClassDB::bind_method(D_METHOD("get_knockback_negation"), &MiniBoss::get_knockback_negation);

    ClassDB::bind_method(D_METHOD("set_slide_speed", "value"), &MiniBoss::set_slide_speed);
    ClassDB::bind_method(D_METHOD("get_slide_speed"), &MiniBoss::get_slide_speed);

    ClassDB::bind_method(D_METHOD("set_slide_damage", "value"), &MiniBoss::set_slide_damage);
    ClassDB::bind_method(D_METHOD("get_slide_damage"), &MiniBoss::get_slide_damage);

    ClassDB::bind_method(D_METHOD("set_slide_stun_scale", "value"), &MiniBoss::set_slide_stun_scale);
    ClassDB::bind_method(D_METHOD("get_slide_stun_scale"), &MiniBoss::get_slide_stun_scale);

    ClassDB::bind_method(D_METHOD("set_slide_knockback", "value"), &MiniBoss::set_slide_knockback);
    ClassDB::bind_method(D_METHOD("get_slide_knockback"), &MiniBoss::get_slide_knockback);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_speed", PROPERTY_HINT_RANGE, "0,1000,1"), "set_move_speed", "get_move_speed");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "close_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_close_range", "get_close_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mid_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_mid_range", "get_mid_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "far_range", PROPERTY_HINT_RANGE, "0,2000,1"), "set_far_range", "get_far_range");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_RANGE, "1,10000,1"), "set_max_health", "get_max_health");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_damage", "get_attack_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_attack_stun_scale", "get_attack_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_attack_knockback", "get_attack_knockback");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_cooldown", PROPERTY_HINT_RANGE, "0.1,10,0.1"), "set_attack_cooldown", "get_attack_cooldown");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stun_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_stun_negation", "get_stun_negation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "knockback_negation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_knockback_negation", "get_knockback_negation");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "slide_speed", PROPERTY_HINT_RANGE, "1,1000,1"), "set_slide_speed", "get_slide_speed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "slide_damage", PROPERTY_HINT_RANGE, "0,1000,1"), "set_slide_damage", "get_slide_damage");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "slide_stun_scale", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_slide_stun_scale", "get_slide_stun_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "slide_knockback", PROPERTY_HINT_RANGE, "0,1000,1"), "set_slide_knockback", "get_slide_knockback");
}

// ==========================================
// CONSTRUCTOR
// ==========================================

MiniBoss::MiniBoss() {
    max_health = 100;
    health = max_health;
    move_speed = 80.0f;
}

MiniBoss::~MiniBoss() {
}

// ==========================================
// READY
// ==========================================

void MiniBoss::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) return;

    health = max_health;

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));
    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    slide_hitbox = get_node<Area2D>(NodePath("slide_hitbox/Area2D"));
    skill1_hitbox = get_node<Area2D>(NodePath("skill1_hitbox/Area2D"));
    skill2_hitbox = get_node<Area2D>(NodePath("skill2_hitbox/Area2D"));
    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));
    body_collision = get_node<CollisionShape2D>(NodePath("CollisionShape2D"));

    target = Object::cast_to<Actor>(get_tree()->get_first_node_in_group("player"));
    random_manager = get_node<Node>(NodePath("/root/RandomManager"));
    if (target != nullptr) {
        if (target->has_signal("normal_attack_started")) target->connect("normal_attack_started", callable_mp(this, &MiniBoss::on_player_attack_started));
        target->connect("tree_exiting", callable_mp(this, &MiniBoss::on_target_exiting));
    }

    slide_hitbox_default_position = slide_hitbox->get_position();
    slide_hitbox->set_monitoring(false);
    slide_hitbox->connect("area_entered", callable_mp(this, &MiniBoss::on_slide_area_entered));

    skill1_hitbox_default_position = skill1_hitbox->get_position();
    skill2_hitbox_default_position = skill2_hitbox->get_position();
    hitbox_default_position = hitbox->get_position();
    // Match the existing sword hitbox's hurtbox mask (John is on layer 4).
    skill1_hitbox->set_collision_layer(0);
    skill2_hitbox->set_collision_layer(0);
    skill1_hitbox->set_collision_mask(hitbox->get_collision_mask());
    skill2_hitbox->set_collision_mask(hitbox->get_collision_mask());
    disable_attack_hitboxes();

    attack_cooldown_timer = 0.0f;

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}

void MiniBoss::_exit_tree() {
    restore_roll_collision(true);
    if (target == nullptr) return;
    Callable attack_callback = callable_mp(this, &MiniBoss::on_player_attack_started);
    Callable exit_callback = callable_mp(this, &MiniBoss::on_target_exiting);
    if (target->is_connected("normal_attack_started", attack_callback)) target->disconnect("normal_attack_started", attack_callback);
    if (target->is_connected("tree_exiting", exit_callback)) target->disconnect("tree_exiting", exit_callback);
}

void MiniBoss::on_target_exiting() {
    restore_roll_collision(true);
    target = nullptr;
}

void MiniBoss::on_player_attack_started(bool facing_left) {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;
    if (roll_cooldown_timer > 0.0f || roll_collision_ignored) return;
    if (get_global_position().distance_to(target->get_global_position()) > close_range) return;
    Ref<SpriteFrames> frames = animated_sprite->get_sprite_frames();
    if (!frames->has_animation("roll") || roll_duration <= 0.0f) return;
    if (static_cast<int>(random_manager->call("random_int", 0, 99)) >= roll_chance) return;

    // Immunity begins synchronously, before John's attack enables its hitbox.
    combat_state = CombatState::ROLLING;
    disable_attack_hitboxes();
    slide_hitbox->set_monitoring(false);
    slide_follow_up_pending = false;
    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));
    roll_start_position = get_global_position();
    roll_target_position = target->get_global_position() + Vector2(facing_left ? roll_behind_distance : -roll_behind_distance, 0);
    roll_speed = roll_start_position.distance_to(roll_target_position) / roll_duration;
    roll_timer = roll_duration;
    roll_cooldown_timer = roll_cooldown;
    add_collision_exception_with(target);
    target->add_collision_exception_with(this);
    roll_collision_ignored = true;
    update_facing(roll_target_position.x - roll_start_position.x);

    double animation_duration = 0.0;
    double fps = frames->get_animation_speed("roll");
    if (fps > 0.0) {
        for (int i = 0; i < frames->get_frame_count("roll"); i++) animation_duration += frames->get_frame_duration("roll", i) / fps;
    }
    animated_sprite->stop();
    animated_sprite->set_speed_scale(animation_duration > 0.0 ? animation_duration / roll_duration : 1.0);
    animated_sprite->play("roll");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}

bool MiniBoss::restore_roll_collision(bool force) {
    if (!roll_collision_ignored) return true;
    if (target != nullptr) {
        CollisionShape2D *player_body = target->get_node<CollisionShape2D>(NodePath("CollisionShape2D"));
        if (!force && player_body != nullptr && !player_body->is_disabled() && !body_collision->is_disabled()) {
            Ref<Shape2D> shape = body_collision->get_shape();
            Ref<Shape2D> player_shape = player_body->get_shape();
            if (shape.is_valid() && player_shape.is_valid() && shape->collide(body_collision->get_global_transform(), player_shape, player_body->get_global_transform())) return false;
        }
        remove_collision_exception_with(target);
        target->remove_collision_exception_with(this);
    }
    roll_collision_ignored = false;
    return true;
}

void MiniBoss::finish_roll() {
    combat_state = CombatState::IDLE;
    roll_timer = 0.0f;
    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));
    restore_roll_collision();
    attack_cooldown_timer = std::max(attack_cooldown_timer, roll_recovery);
    if (target != nullptr) update_facing(target->get_global_position().x - get_global_position().x);
    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}

// ==========================================
// BUFF
// ==========================================

void MiniBoss::buff() {
    if (combat_state == CombatState::ROLLING || combat_state == CombatState::BUFFING || combat_state == CombatState::HEALING || combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) return;

    combat_state = CombatState::BUFFING;
    buff_has_applied = false;

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("buff");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}

// ==========================================
// HEAL
// ==========================================

void MiniBoss::heal() {
    if (combat_state == CombatState::ROLLING || combat_state == CombatState::HEALING || combat_state == CombatState::BUFFING || combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) return;

    combat_state = CombatState::HEALING;
    heal_has_applied = false;
    has_healed = true;

    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("heal");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}

// ==========================================
// SLIDE
// ==========================================

void MiniBoss::start_slide() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;

    combat_state = CombatState::SLIDING;
    slide_aligned = false;
    slide_has_hit = false;
    slide_damage_pending = false;
    slide_follow_up_pending = false;
    slide_direction = 0.0f;
    slide_target_x = 0.0f;
    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));
    disable_attack_hitboxes();
    slide_hitbox->set_monitoring(false);
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("run");
}

void MiniBoss::on_slide_area_entered(Area2D *area) {
    if (health <= 0 || combat_state != CombatState::SLIDING || !slide_aligned || slide_has_hit || slide_follow_up_pending) return;
    if (target == nullptr || target->get_health() <= 0) return;

    for (Node *node = area; node != nullptr; node = node->get_parent()) {
        if (node != target) continue;
        slide_has_hit = true;
        slide_damage_pending = true;
        finish_slide();
        break;
    }
}

// Called from physics processing, never directly from Area2D entry callbacks.
void MiniBoss::apply_pending_slide_damage() {
    if (!slide_damage_pending) return;
    slide_damage_pending = false;
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    target->take_damage(slide_damage, slide_stun_scale, slide_knockback, this);
}

void MiniBoss::check_slide_contact() {
    if (health <= 0 || combat_state != CombatState::SLIDING || !slide_aligned || slide_has_hit || slide_follow_up_pending) return;
    if (target == nullptr || target->get_health() <= 0) return;
    Area2D *player_hurtbox = target->get_node<Area2D>(NodePath("hurtbox/Area2D"));
    CollisionShape2D *slide_shape = slide_hitbox->get_node<CollisionShape2D>(NodePath("CollisionShape2D"));
    CollisionShape2D *player_shape = target->get_node<CollisionShape2D>(NodePath("hurtbox/Area2D/CollisionShape2D"));
    if (player_hurtbox == nullptr || slide_shape == nullptr || player_shape == nullptr) return;
    if (!player_hurtbox->is_monitorable() || slide_shape->is_disabled() || player_shape->is_disabled()) return;
    if ((slide_hitbox->get_collision_mask() & player_hurtbox->get_collision_layer()) == 0) return;
    Ref<Shape2D> shape = slide_shape->get_shape();
    Ref<Shape2D> hurt_shape = player_shape->get_shape();
    if (shape.is_valid() && hurt_shape.is_valid() && shape->collide(slide_shape->get_global_transform(), hurt_shape, player_shape->get_global_transform())) on_slide_area_entered(player_hurtbox);
}

void MiniBoss::update_slide_follow_up() {
    if (!slide_follow_up_pending || health <= 0) return;
    if (target == nullptr || target->get_health() <= 0) {
        slide_follow_up_pending = false;
        combat_state = CombatState::IDLE;
        animated_sprite->play("idle");
        return;
    }
    Vector2 offset = target->get_global_position() - get_global_position();
    if (offset.length() > close_range) {
        slide_follow_up_pending = false;
        combat_state = CombatState::MOVING;
        animated_sprite->play("run");
    }
    else if (Math::abs(offset.y) <= slide_align_tolerance) choose_attack_moveset();
    else animated_sprite->play("run");
}

void MiniBoss::finish_slide() {
    slide_aligned = false;
    slide_follow_up_pending = true;
    // Also called from area_entered: defer changes to the physics server.
    slide_hitbox->set_deferred("monitoring", false);
    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));
    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
}

void MiniBoss::choose_attack_moveset() {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING && !(combat_state == CombatState::SLIDING && slide_follow_up_pending)) return;

    Vector2 offset = target->get_global_position() - get_global_position();
    if (offset.length() > close_range || Math::abs(offset.y) > slide_align_tolerance) return;
    if (!slide_follow_up_pending && attack_cooldown_timer > 0.0f) return;

    slide_follow_up_pending = false;
    combat_state = CombatState::IDLE;

    float health_ratio = static_cast<float>(health) / static_cast<float>(max_health);
    int current_skill_1_chance = health_ratio < 0.3f ? 20 : skill_1_chance;

    int roll = static_cast<int>(random_manager->call("random_int", 0, 99));
    if (roll < attack_chance) attack();
    else if (roll < attack_chance + current_skill_1_chance) skill_1();
    else skill_2();
}

// ==========================================
// DECIDE ACTION
// ==========================================

void MiniBoss::decide_action() {
    if (target == nullptr || target->get_health() <= 0) {
        combat_state = CombatState::IDLE;
        return;
    }

    if (combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) return;

    Vector2 player_position = target->get_global_position();
    Vector2 boss_position = get_global_position();

    float total_distance = boss_position.distance_to(player_position);
    float direction_x = player_position.x - boss_position.x;

    update_facing(direction_x);

    // ==========================================
    // ATTACK COOLDOWN
    // ==========================================

    if (attack_cooldown_timer > 0.0f) {
        combat_state = CombatState::IDLE;
        return;
    }

    // ==========================================
    // OUTSIDE DETECTION
    // ==========================================

    if (total_distance > far_range) {
        combat_state = CombatState::IDLE;
        return;
    }

    // ==========================================
    // FAR RANGE
    // ==========================================

    if (total_distance > mid_range) {
        float boss_health_ratio = static_cast<float>(health) / static_cast<float>(max_health);
        float player_health_ratio = static_cast<float>(target->get_health()) / static_cast<float>(target->get_max_health());

        if (boss_health_ratio <= 0.4f && !has_healed) {
            heal();
            return;
        }

        if (boss_health_ratio > 0.4f && player_health_ratio > 0.5f && !is_buffed) {
            buff();
            return;
        }

        combat_state = CombatState::IDLE;
        return;
    }

    // ==========================================
    // MID RANGE
    // ==========================================

    if (total_distance > close_range) {
        if (combat_state == CombatState::MOVING) return;

        int choice = static_cast<int>(random_manager->call("random_int", 0, 1));

        if (choice == 0) {
            combat_state = CombatState::MOVING;
        }
        else {
            start_slide();
        }

        return;
    }

    // ==========================================
    // CLOSE RANGE
    // ==========================================

    if (Math::abs(player_position.y - boss_position.y) > slide_align_tolerance) {
        combat_state = CombatState::MOVING;
        return;
    }

    choose_attack_moveset();
}

// ==========================================
// TAKE DAMAGE
// ==========================================

void MiniBoss::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0 || combat_state == CombatState::ROLLING) return;

    health -= damage;

    if (health < 0) {
        health = 0;
    }

    UtilityFunctions::print("MiniBoss took ", damage, " damage | HP: ", health, "/", max_health);

    if (health <= 0) {
        die(false);
        return;
    }

    float final_stun = damage * stun_scale * (1.0f - stun_negation);
    float final_knockback = damage * knockback * (1.0f - knockback_negation);

    if (attacker != nullptr) {
        Vector2 knockback_direction = (get_global_position() - attacker->get_global_position()).normalized();
        knockback_velocity = knockback_direction * final_knockback;
    }

    bool can_be_stunned = combat_state == CombatState::IDLE || combat_state == CombatState::MOVING;

    if (can_be_stunned && final_stun > 0.0f) {
        stun_timer = final_stun;
        combat_state = CombatState::STUNNED;

        disable_attack_hitboxes();

        if (final_stun >= min_stun_animation_duration) {
            Ref<SpriteFrames> sprite_frames = animated_sprite->get_sprite_frames();

            int frame_count = sprite_frames->get_frame_count(StringName("hitted"));
            double animation_fps = sprite_frames->get_animation_speed(StringName("hitted"));

            if (frame_count > 0 && animation_fps > 0.0) {
                double animation_duration = static_cast<double>(frame_count) / animation_fps;

                animated_sprite->set_speed_scale(animation_duration / final_stun);
                animated_sprite->play("hitted");
            }
        }
    }
}

// ==========================================
// DIE
// ==========================================

void MiniBoss::die(bool play_sound) {
    restore_roll_collision(true);
    roll_timer = 0.0f;
    slide_hitbox->set_deferred("monitoring", false);
    slide_aligned = false;
    slide_follow_up_pending = false;
    slide_has_hit = false;
    slide_damage_pending = false;
    hitbox->set_deferred("monitoring", false);
    skill1_hitbox->set_deferred("monitoring", false);
    skill2_hitbox->set_deferred("monitoring", false);

    hurtbox->set_deferred("monitorable", false);
    body_collision->set_deferred("disabled", true);

    knockback_velocity = Vector2(0, 0);
    set_velocity(Vector2(0, 0));

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("die");
    animated_sprite->set_frame_and_progress(0, 0.0f);

    UtilityFunctions::print("MiniBoss died");
}

// ==========================================
// ATTACK
// ==========================================

void MiniBoss::attack() { start_attack("attack", hitbox); }
void MiniBoss::skill_1() { start_attack("skill_1", skill1_hitbox); }
void MiniBoss::skill_2() { start_attack("skill_2", skill2_hitbox); }

void MiniBoss::disable_attack_hitboxes() {
    hitbox->set_monitoring(false);
    skill1_hitbox->set_monitoring(false);
    skill2_hitbox->set_monitoring(false);
}

void MiniBoss::start_attack(const StringName &animation, Area2D *active_hitbox) {
    if (health <= 0 || target == nullptr || target->get_health() <= 0) return;
    if (combat_state != CombatState::IDLE && combat_state != CombatState::MOVING) return;

    combat_state = CombatState::ATTACKING;
    slide_follow_up_pending = false;
    attack_has_hit = false;
    skill_1_first_hit = false;
    skill_1_second_hit = false;
    skill_2_has_hit = false;
    disable_attack_hitboxes();
    slide_hitbox->set_monitoring(false);
    // Track overlaps during wind-up; damage is gated by frame windows below.
    active_hitbox->set_monitoring(true);
    update_facing(target->get_global_position().x - get_global_position().x);
    set_velocity(Vector2(0, 0));
    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play(animation);
    animated_sprite->set_frame_and_progress(0, 0.0f);
}

void MiniBoss::apply_attack_hit(Area2D *active_hitbox, bool &has_hit, int damage, float stun_scale, float knockback) {
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
// UPDATE FACING
// ==========================================

void MiniBoss::update_facing(float direction_x) {
    if (direction_x == 0.0f) return;
    bool face_left = direction_x < 0.0f;
    animated_sprite->set_flip_h(face_left);
    Vector2 hitbox_position = hitbox_default_position;
    Vector2 slide_position = slide_hitbox_default_position;
    Vector2 skill1_position = skill1_hitbox_default_position;
    Vector2 skill2_position = skill2_hitbox_default_position;
    if (face_left) {
        hitbox_position.x = -hitbox_position.x;
        slide_position.x = -slide_position.x;
        skill1_position.x = -skill1_position.x;
        skill2_position.x = -skill2_position.x;
    }
    hitbox->set_position(hitbox_position);
    slide_hitbox->set_position(slide_position);
    skill1_hitbox->set_position(skill1_position);
    skill2_hitbox->set_position(skill2_position);
    // Mirror child shape offsets too: the slide shape is offset inside Area2D.
    for (Area2D *area : {hitbox, slide_hitbox, skill1_hitbox, skill2_hitbox}) {
        Vector2 area_scale = area->get_scale();
        area_scale.x = Math::abs(area_scale.x) * (face_left ? -1.0f : 1.0f);
        area->set_scale(area_scale);
    }
}

// ==========================================
// PHYSICS PROCESS
// ==========================================

void MiniBoss::_physics_process(double delta) {
    if (delta <= 0.0) return;
    if (Engine::get_singleton()->is_editor_hint()) return;

    // ==========================================
    // DEAD
    // ==========================================

    if (health <= 0) {
        if (!animated_sprite->is_playing()) {
            // DEBUG ONLY:
            // Later replace this with:
            // drop_item();
            // queue_free();

            health = max_health;
            has_healed = false;

            hurtbox->set_monitorable(true);
            body_collision->set_deferred("disabled", false);

            combat_state = CombatState::IDLE;
            attack_cooldown_timer = 0.0f;
            knockback_velocity = Vector2(0, 0);

            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");

            UtilityFunctions::print("MiniBoss revived for debug | HP: ", health, "/", max_health);
        }

        return;
    }

    apply_pending_slide_damage();
    if (health <= 0) return;

    // ==========================================
    // ATTACK COOLDOWN
    // ==========================================

    if (attack_cooldown_timer > 0.0f) {
        attack_cooldown_timer -= delta;

        if (attack_cooldown_timer < 0.0f) {
            attack_cooldown_timer = 0.0f;
        }
    }

    roll_cooldown_timer = std::max(0.0f, roll_cooldown_timer - static_cast<float>(delta));
    if (combat_state == CombatState::ROLLING) {
        if (target == nullptr || target->get_health() <= 0) {
            finish_roll();
            return;
        }
        Vector2 offset = roll_target_position - get_global_position();
        float step = std::min(static_cast<float>(delta), roll_timer);
        float distance = std::min(static_cast<float>(offset.length()), roll_speed * step);
        Ref<KinematicCollision2D> collision = move_and_collide(offset.normalized() * distance);
        roll_timer -= step;
        if (collision.is_valid() || roll_timer <= 0.0f || get_global_position().distance_to(roll_target_position) <= 0.5f) finish_roll();
        return;
    }

    // If a wall stopped the roll inside John, retreat until the bodies separate.
    // Immunity has already ended; never extend it while waiting for separation.
    if (roll_collision_ignored && !restore_roll_collision()) {
        Vector2 escape = roll_start_position - get_global_position();
        if (escape.length() < 1.0f && target != nullptr) escape = get_global_position() - target->get_global_position();
        if (escape.length() < 0.001f) escape = Vector2(1, 0);
        move_and_collide(escape.normalized() * std::min(static_cast<float>(escape.length()), move_speed * static_cast<float>(delta)));
        restore_roll_collision();
        return;
    }

    Vector2 movement_velocity = Vector2(0, 0);

    // ==========================================
    // STUN
    // ==========================================

    if (combat_state == CombatState::STUNNED) {
        disable_attack_hitboxes();
        stun_timer -= delta;

        if (stun_timer <= 0.0f) {
            stun_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }
    }

    // ==========================================
    // AI DECISION
    // ==========================================

    if (combat_state != CombatState::STUNNED && combat_state != CombatState::ATTACKING && combat_state != CombatState::BUFFING && combat_state != CombatState::HEALING && combat_state != CombatState::SLIDING) {
        decide_action();
    }

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
            // Never retreat to the edge when already close. Leave a small margin on approach.
            float x_distance = std::max(0.0f, static_cast<float>(Math::abs(offset.x)) - close_range * 0.9f);
            Vector2 approach = Vector2(offset.x >= 0.0f ? x_distance : -x_distance, offset.y);
            if (boss_position.distance_to(player_position) <= close_range) approach.x = 0.0f;
            float speed = std::min(move_speed, static_cast<float>(approach.length() / delta));
            movement_velocity = approach.normalized() * speed;

            update_facing(player_position.x - boss_position.x);

            if (animated_sprite->get_animation() != StringName("run")) {
                animated_sprite->set_speed_scale(1.0f);
                animated_sprite->play("run");
            }
        }
    }

    // ==========================================
    // BUFFING
    // ==========================================

    else if (combat_state == CombatState::BUFFING) {
        int buff_frame = animated_sprite->get_frame();

        if (buff_frame >= 7 && !buff_has_applied) {
            attack_damage = static_cast<int>(attack_damage * buff_damage_multiplier);

            buff_has_applied = true;
            is_buffed = true;

            UtilityFunctions::print("MiniBoss buffed | Attack damage: ", attack_damage);
        }

        if (!animated_sprite->is_playing()) {
            combat_state = CombatState::IDLE;
        }
    }

    // ==========================================
    // HEALING
    // ==========================================

    else if (combat_state == CombatState::HEALING) {
        int heal_frame = animated_sprite->get_frame();

        if (heal_frame >= 8 && !heal_has_applied) {
            health += heal_amount;

            if (health > max_health) {
                health = max_health;
            }

            heal_has_applied = true;

            UtilityFunctions::print("MiniBoss healed | HP: ", health, "/", max_health);
        }

        if (!animated_sprite->is_playing()) {
            combat_state = CombatState::IDLE;
        }
    }

    // ==========================================
    // SLIDING
    // ==========================================

    else if (combat_state == CombatState::SLIDING) {
        if (target == nullptr || target->get_health() <= 0) {
            finish_slide();
            slide_follow_up_pending = false;
            combat_state = CombatState::IDLE;
            animated_sprite->play("idle");
        }
        else {
            Vector2 boss_position = get_global_position();
            Vector2 player_position = target->get_global_position();
            float y_difference = player_position.y - boss_position.y;
            float step = static_cast<float>(delta);

            // Align before the slide and again before choosing its follow-up.
            if (slide_follow_up_pending && boss_position.distance_to(player_position) > close_range) {
                slide_follow_up_pending = false;
                combat_state = CombatState::MOVING;
                animated_sprite->play("run");
            }
            else if (!slide_aligned) {
                update_facing(player_position.x - boss_position.x);
                if (Math::abs(y_difference) > slide_align_tolerance) {
                    float y_speed = std::min(move_speed, Math::abs(y_difference) / step);
                    movement_velocity = Vector2(0, y_difference > 0.0f ? y_speed : -y_speed);
                    animated_sprite->set_speed_scale(1.0f);
                    if (animated_sprite->get_animation() != StringName("run") || !animated_sprite->is_playing()) animated_sprite->play("run");
                }
                else if (slide_follow_up_pending) {
                    choose_attack_moveset();
                }
                else {
                    // Lock X once. John dodging afterwards does not move this destination.
                    slide_target_x = player_position.x;
                    slide_direction = slide_target_x >= boss_position.x ? 1.0f : -1.0f;
                    float slide_distance = Math::abs(slide_target_x - boss_position.x);
                    if (slide_distance <= 0.001f) {
                        finish_slide();
                        update_slide_follow_up();
                    }
                    else {
                        float slide_duration = slide_distance / slide_speed;
                        Ref<SpriteFrames> frames = animated_sprite->get_sprite_frames();
                        double fps = frames->get_animation_speed("slide");
                        double animation_duration = 0.0;
                        if (fps > 0.0) {
                            for (int i = 0; i < frames->get_frame_count("slide"); i++) animation_duration += frames->get_frame_duration("slide", i) / fps;
                        }

                        slide_aligned = true;
                        slide_hitbox->set_monitoring(true);
                        animated_sprite->stop();
                        animated_sprite->set_speed_scale(animation_duration > 0.0 ? animation_duration / slide_duration : 1.0);
                        animated_sprite->play("slide");
                        animated_sprite->set_frame_and_progress(0, 0.0f);
                        movement_velocity = Vector2(slide_direction * std::min(slide_speed, slide_distance / step), 0);
                    }
                }
            }
            else {
                // Use current shape transforms instead of the previous physics overlap list.
                check_slide_contact();
                apply_pending_slide_damage();

                float remaining_distance = (slide_target_x - boss_position.x) * slide_direction;
                bool blocked = get_slide_collision_count() > 0 && Math::abs(get_velocity().x) < 0.001f;
                if (!slide_follow_up_pending && (remaining_distance <= 0.001f || blocked)) finish_slide();
                if (slide_follow_up_pending) {
                    update_slide_follow_up();
                }
                else {
                    movement_velocity = Vector2(slide_direction * std::min(slide_speed, remaining_distance / step), 0);
                }
            }
        }
    }

    // ==========================================
    // ATTACKING
    // ==========================================

    else if (combat_state == CombatState::ATTACKING) {
        int frame = animated_sprite->get_frame();
        StringName animation = animated_sprite->get_animation();
        if (animated_sprite->is_playing()) {
            if (animation == StringName("attack") && frame >= 6 && frame <= 7) {
                apply_attack_hit(hitbox, attack_has_hit, attack_damage, attack_stun_scale, attack_knockback);
            }
            else if (animation == StringName("skill_1")) {
                if (frame >= 4 && frame <= 5) apply_attack_hit(skill1_hitbox, skill_1_first_hit, skill_1_damage, skill_1_stun_scale, skill_1_knockback);
                else if (frame >= 12 && frame <= 13) apply_attack_hit(skill1_hitbox, skill_1_second_hit, skill_1_damage, skill_1_stun_scale, skill_1_knockback);
            }
            else if (animation == StringName("skill_2") && frame >= 3 && frame <= 4) {
                apply_attack_hit(skill2_hitbox, skill_2_has_hit, skill_2_damage, skill_2_stun_scale, skill_2_knockback);
            }
        }

        if (health > 0 && !animated_sprite->is_playing()) {
            disable_attack_hitboxes();
            combat_state = CombatState::IDLE;
            attack_cooldown_timer = attack_cooldown;
        }
    }

    // ==========================================
    // KNOCKBACK
    // ==========================================

    if (combat_state == CombatState::SLIDING) knockback_velocity = Vector2(0, 0);
    if (knockback_velocity.length() > 0.0f) {
        knockback_velocity = knockback_velocity.move_toward(Vector2(0, 0), 500.0f * delta);

        if (knockback_velocity.length() < 1.0f) {
            knockback_velocity = Vector2(0, 0);
        }
    }

    // ==========================================
    // FINAL MOVEMENT
    // ==========================================

    bool sliding_before_move = combat_state == CombatState::SLIDING && slide_aligned;
    check_slide_contact();
    apply_pending_slide_damage();
    if (health <= 0) return;
    if (sliding_before_move && slide_follow_up_pending) {
        movement_velocity = Vector2(0, 0);
        update_slide_follow_up();
    }
    set_velocity(movement_velocity + knockback_velocity);
    move_and_slide();

    if (combat_state == CombatState::SLIDING && slide_aligned) {
        check_slide_contact();
        apply_pending_slide_damage();
        if (health <= 0) return;
        float remaining_distance = (slide_target_x - get_global_position().x) * slide_direction;
        bool blocked = get_slide_collision_count() > 0 && Math::abs(get_velocity().x) < 0.001f;
        if (!slide_follow_up_pending && (remaining_distance <= 0.001f || blocked)) finish_slide();
        update_slide_follow_up();
    }

}

// ==========================================
// SETTERS / GETTERS
// ==========================================

void MiniBoss::set_move_speed(float value) { move_speed = value; }

float MiniBoss::get_move_speed() const { return move_speed; }

void MiniBoss::set_close_range(float value) { close_range = value; }

float MiniBoss::get_close_range() const { return close_range; }

void MiniBoss::set_mid_range(float value) { mid_range = value; }

float MiniBoss::get_mid_range() const { return mid_range; }

void MiniBoss::set_far_range(float value) { far_range = value; }

float MiniBoss::get_far_range() const { return far_range; }

void MiniBoss::set_max_health(int value) { max_health = value; }

void MiniBoss::set_attack_damage(int value) { attack_damage = value; }

int MiniBoss::get_attack_damage() const { return attack_damage; }

void MiniBoss::set_attack_stun_scale(float value) { attack_stun_scale = value; }

float MiniBoss::get_attack_stun_scale() const { return attack_stun_scale; }

void MiniBoss::set_attack_knockback(float value) { attack_knockback = value; }

float MiniBoss::get_attack_knockback() const { return attack_knockback; }

void MiniBoss::set_attack_cooldown(float value) { attack_cooldown = value; }

float MiniBoss::get_attack_cooldown() const { return attack_cooldown; }

void MiniBoss::set_stun_negation(float value) { stun_negation = value; }

float MiniBoss::get_stun_negation() const { return stun_negation; }

void MiniBoss::set_knockback_negation(float value) { knockback_negation = value; }

float MiniBoss::get_knockback_negation() const { return knockback_negation; }

void MiniBoss::set_slide_speed(float value) { slide_speed = std::max(value, 1.0f); }

float MiniBoss::get_slide_speed() const { return slide_speed; }

void MiniBoss::set_slide_damage(int value) { slide_damage = value; }

int MiniBoss::get_slide_damage() const { return slide_damage; }

void MiniBoss::set_slide_stun_scale(float value) { slide_stun_scale = value; }

float MiniBoss::get_slide_stun_scale() const { return slide_stun_scale; }

void MiniBoss::set_slide_knockback(float value) { slide_knockback = value; }

float MiniBoss::get_slide_knockback() const { return slide_knockback; }

void MiniBoss::set_detection_range(float value) { detection_range = value; }
float MiniBoss::get_detection_range() const { return detection_range; }
