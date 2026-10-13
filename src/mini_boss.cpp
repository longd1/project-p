#include "mini_boss.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
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
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    health = max_health;

    animated_sprite = get_node<AnimatedSprite2D>(NodePath("AnimatedSprite2D"));
    hitbox = get_node<Area2D>(NodePath("hitbox/Area2D"));
    hurtbox = get_node<Area2D>(NodePath("hurtbox/Area2D"));
    body_collision = get_node<CollisionShape2D>(NodePath("CollisionShape2D"));

    target = Object::cast_to<Actor>(get_tree()->get_first_node_in_group("player"));
    random_manager = get_node<Node>(NodePath("/root/RandomManager"));

    hitbox_default_position = hitbox->get_position();
    hitbox->set_monitoring(false);

    attack_cooldown_timer = 0.0f;

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("idle");
}


// ==========================================
// BUFF
// ==========================================

void MiniBoss::buff() {
    if (combat_state == CombatState::BUFFING || combat_state == CombatState::HEALING || combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) {
        return;
    }

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
    if (combat_state == CombatState::HEALING || combat_state == CombatState::BUFFING || combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) {
        return;
    }

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
    if (health <= 0) {
        return;
    }

    if (combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED || combat_state == CombatState::SLIDING) {
        return;
    }

    combat_state = CombatState::SLIDING;

    slide_aligned = false;
    slide_has_hit = false;
    slide_direction = 0.0f;
    slide_target_x = 0.0f;

    hitbox->set_monitoring(false);

    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("run");
}


// ==========================================
// DECIDE ACTION
// ==========================================

void MiniBoss::decide_action() {
    if (target == nullptr) {
        combat_state = CombatState::IDLE;
        return;
    }

    if (combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) {
        return;
    }

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
    // ATTACK POSITIONS
    // ==========================================

    Vector2 left_attack_position = Vector2(player_position.x - close_range, player_position.y);
    Vector2 right_attack_position = Vector2(player_position.x + close_range, player_position.y);
    Vector2 attack_position = boss_position.x <= player_position.x ? left_attack_position : right_attack_position;

    float distance_to_attack_position = boss_position.distance_to(attack_position);

    // ==========================================
    // MID RANGE
    // ==========================================

    if (total_distance > close_range) {
        if (combat_state == CombatState::MOVING) {
            return;
        }

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

    const float attack_position_tolerance = 5.0f;

    if (distance_to_attack_position > attack_position_tolerance) {
        combat_state = CombatState::MOVING;
        return;
    }

    attack();
}


// ==========================================
// TAKE DAMAGE
// ==========================================

void MiniBoss::take_damage(int damage, float stun_scale, float knockback, Actor *attacker) {
    if (health <= 0) {
        return;
    }

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

        hitbox->set_monitoring(false);

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
    hitbox->set_monitoring(false);

    hurtbox->set_monitorable(false);
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

void MiniBoss::attack() {
    if (health <= 0) {
        return;
    }

    if (combat_state == CombatState::ATTACKING || combat_state == CombatState::STUNNED) {
        return;
    }

    combat_state = CombatState::ATTACKING;

    attack_has_hit = false;
    hitbox->set_monitoring(false);

    animated_sprite->stop();
    animated_sprite->set_speed_scale(1.0f);
    animated_sprite->play("attack");
    animated_sprite->set_frame_and_progress(0, 0.0f);
}


// ==========================================
// UPDATE FACING
// ==========================================

void MiniBoss::update_facing(float direction_x) {
    if (direction_x < 0.0f) {
        animated_sprite->set_flip_h(true);

        Vector2 hitbox_position = hitbox_default_position;
        hitbox_position.x = -hitbox_default_position.x;

        hitbox->set_position(hitbox_position);
    }
    else if (direction_x > 0.0f) {
        animated_sprite->set_flip_h(false);
        hitbox->set_position(hitbox_default_position);
    }
}


// ==========================================
// PHYSICS PROCESS
// ==========================================

void MiniBoss::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

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

    // ==========================================
    // ATTACK COOLDOWN
    // ==========================================

    if (attack_cooldown_timer > 0.0f) {
        attack_cooldown_timer -= delta;

        if (attack_cooldown_timer < 0.0f) {
            attack_cooldown_timer = 0.0f;
        }
    }

    Vector2 movement_velocity = Vector2(0, 0);

    // ==========================================
    // STUN
    // ==========================================

    if (combat_state == CombatState::STUNNED) {
        hitbox->set_monitoring(false);
        stun_timer -= delta;

        if (stun_timer <= 0.0f) {
            stun_timer = 0.0f;
            combat_state = CombatState::IDLE;
        }
    }

    // ==========================================
    // AI DECISION
    // ==========================================

    if (combat_state != CombatState::STUNNED &&
        combat_state != CombatState::ATTACKING &&
        combat_state != CombatState::BUFFING &&
        combat_state != CombatState::HEALING &&
        combat_state != CombatState::SLIDING) {
        decide_action();
    }

    // ==========================================
    // IDLE
    // ==========================================

    if (combat_state == CombatState::IDLE) {
        hitbox->set_monitoring(false);

        if (animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->set_speed_scale(1.0f);
            animated_sprite->play("idle");
        }
    }

    // ==========================================
    // MOVING
    // ==========================================

    else if (combat_state == CombatState::MOVING) {
        hitbox->set_monitoring(false);

        if (target != nullptr) {
            Vector2 player_position = target->get_global_position();
            Vector2 boss_position = get_global_position();

            Vector2 left_attack_position = Vector2(player_position.x - close_range, player_position.y);
            Vector2 right_attack_position = Vector2(player_position.x + close_range, player_position.y);
            Vector2 attack_position = boss_position.x <= player_position.x ? left_attack_position : right_attack_position;

            Vector2 direction = (attack_position - boss_position).normalized();
            movement_velocity = direction * move_speed;

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
        if (target == nullptr) {
            combat_state = CombatState::IDLE;
        }
        else {
            Vector2 boss_position = get_global_position();
            Vector2 player_position = target->get_global_position();
            float y_difference = player_position.y - boss_position.y;

            // ==========================================
            // ALIGN WITH PLAYER
            // ==========================================

            if (!slide_aligned) {
                if (Math::abs(y_difference) > slide_align_tolerance) {
                    float y_direction = y_difference > 0.0f ? 1.0f : -1.0f;
                    movement_velocity = Vector2(0, y_direction * move_speed);

                    if (animated_sprite->get_animation() != StringName("run")) {
                        animated_sprite->set_speed_scale(1.0f);
                        animated_sprite->play("run");
                    }
                }
                else {
                    slide_aligned = true;
                    slide_direction = player_position.x >= boss_position.x ? 1.0f : -1.0f;
                    slide_target_x = player_position.x;

                    update_facing(slide_direction);

                    float slide_distance = Math::abs(slide_target_x - boss_position.x);
                    float slide_duration = slide_distance / slide_speed;

                    Ref<SpriteFrames> sprite_frames = animated_sprite->get_sprite_frames();
                    int frame_count = sprite_frames->get_frame_count(StringName("slide"));
                    double animation_fps = sprite_frames->get_animation_speed(StringName("slide"));
                    double animation_duration = 0.0;

                    for (int i = 0; i < frame_count; i++) {
                        animation_duration += sprite_frames->get_frame_duration(StringName("slide"), i) / animation_fps;
                    }

                    animated_sprite->stop();

                    if (slide_duration > 0.0f && animation_duration > 0.0) {
                        animated_sprite->set_speed_scale(animation_duration / slide_duration);
                    }
                    else {
                        animated_sprite->set_speed_scale(1.0f);
                    }

                    animated_sprite->play("slide");
                    animated_sprite->set_frame_and_progress(0, 0.0f);
                }
            }

            // ==========================================
            // SLIDE
            // ==========================================

            else {
                float remaining_distance = Math::abs(slide_target_x - boss_position.x);

                if (remaining_distance > 2.0f) {
                    float current_slide_speed = slide_speed;

                    if (remaining_distance < slide_speed * static_cast<float>(delta)) {
                        current_slide_speed = remaining_distance / static_cast<float>(delta);
                    }

                    movement_velocity = Vector2(slide_direction * current_slide_speed, 0);
                }

                hitbox->set_monitoring(true);

                TypedArray<Area2D> overlapping_areas = hitbox->get_overlapping_areas();

                for (int i = 0; i < overlapping_areas.size(); i++) {
                    Area2D *area = Object::cast_to<Area2D>(overlapping_areas[i]);

                    if (area == nullptr || slide_has_hit) {
                        continue;
                    }

                    Node *hit_target = area;

                    while (hit_target != nullptr) {
                        Actor *actor = Object::cast_to<Actor>(hit_target);

                        if (actor != nullptr && actor != this) {
                            actor->take_damage(slide_damage, slide_stun_scale, slide_knockback, this);

                            slide_has_hit = true;
                            break;
                        }

                        hit_target = hit_target->get_parent();
                    }
                }

                // ==========================================
                // SLIDE FINISHED
                // ==========================================

                    if (remaining_distance <= 2.0f || slide_has_hit) {
                        movement_velocity = Vector2(0, 0);
                        hitbox->set_monitoring(false);

                        combat_state = CombatState::IDLE;
                        slide_aligned = false;
                        slide_has_hit = false;

                        animated_sprite->set_speed_scale(1.0f);
                        animated_sprite->play("idle");
                    }
            }
        }
    }

    // ==========================================
    // ATTACKING
    // ==========================================

    else if (combat_state == CombatState::ATTACKING) {
        int attack_frame = animated_sprite->get_frame();

        // ==========================================
        // HIT WINDOW
        // ==========================================

        if (attack_frame >= 6 && attack_frame <= 7) {
            hitbox->set_monitoring(true);

            TypedArray<Area2D> overlapping_areas = hitbox->get_overlapping_areas();

            for (int i = 0; i < overlapping_areas.size(); i++) {
                Area2D *area = Object::cast_to<Area2D>(overlapping_areas[i]);

                if (area == nullptr || attack_has_hit) {
                    continue;
                }

                Node *hit_target = area;

                while (hit_target != nullptr) {
                    Actor *actor = Object::cast_to<Actor>(hit_target);

                    if (actor != nullptr && actor != this) {
                        actor->take_damage(attack_damage, attack_stun_scale, attack_knockback, this);

                        attack_has_hit = true;
                        break;
                    }

                    hit_target = hit_target->get_parent();
                }
            }
        }
        else {
            hitbox->set_monitoring(false);
        }

        // ==========================================
        // ATTACK FINISHED
        // ==========================================

        if (!animated_sprite->is_playing()) {
            hitbox->set_monitoring(false);

            combat_state = CombatState::IDLE;
            attack_cooldown_timer = attack_cooldown;
        }
    }

    // ==========================================
    // KNOCKBACK
    // ==========================================

    if (knockback_velocity.length() > 0.0f) {
        knockback_velocity = knockback_velocity.move_toward(Vector2(0, 0), 500.0f * delta);

        if (knockback_velocity.length() < 1.0f) {
            knockback_velocity = Vector2(0, 0);
        }
    }

    // ==========================================
    // FINAL MOVEMENT
    // ==========================================

    set_velocity(movement_velocity + knockback_velocity);
    move_and_slide();
    if (combat_state == CombatState::SLIDING && slide_aligned && get_slide_collision_count() > 0) {
        hitbox->set_monitoring(false);

        combat_state = CombatState::IDLE;
        slide_aligned = false;
        slide_has_hit = false;

        animated_sprite->set_speed_scale(1.0f);
        animated_sprite->play("idle");
}
}


// ==========================================
// SETTERS / GETTERS
// ==========================================

void MiniBoss::set_move_speed(float value) {
    move_speed = value;
}

float MiniBoss::get_move_speed() const {
    return move_speed;
}


void MiniBoss::set_close_range(float value) {
    close_range = value;
}

float MiniBoss::get_close_range() const {
    return close_range;
}


void MiniBoss::set_mid_range(float value) {
    mid_range = value;
}

float MiniBoss::get_mid_range() const {
    return mid_range;
}


void MiniBoss::set_far_range(float value) {
    far_range = value;
}

float MiniBoss::get_far_range() const {
    return far_range;
}


void MiniBoss::set_max_health(int value) {
    max_health = value;
}


void MiniBoss::set_attack_damage(int value) {
    attack_damage = value;
}

int MiniBoss::get_attack_damage() const {
    return attack_damage;
}


void MiniBoss::set_attack_stun_scale(float value) {
    attack_stun_scale = value;
}

float MiniBoss::get_attack_stun_scale() const {
    return attack_stun_scale;
}


void MiniBoss::set_attack_knockback(float value) {
    attack_knockback = value;
}

float MiniBoss::get_attack_knockback() const {
    return attack_knockback;
}


void MiniBoss::set_attack_cooldown(float value) {
    attack_cooldown = value;
}

float MiniBoss::get_attack_cooldown() const {
    return attack_cooldown;
}


void MiniBoss::set_stun_negation(float value) {
    stun_negation = value;
}

float MiniBoss::get_stun_negation() const {
    return stun_negation;
}


void MiniBoss::set_knockback_negation(float value) {
    knockback_negation = value;
}

float MiniBoss::get_knockback_negation() const {
    return knockback_negation;
}


void MiniBoss::set_slide_speed(float value) {
    slide_speed = value;
}

float MiniBoss::get_slide_speed() const {
    return slide_speed;
}


void MiniBoss::set_slide_damage(int value) {
    slide_damage = value;
}

int MiniBoss::get_slide_damage() const {
    return slide_damage;
}


void MiniBoss::set_slide_stun_scale(float value) {
    slide_stun_scale = value;
}

float MiniBoss::get_slide_stun_scale() const {
    return slide_stun_scale;
}


void MiniBoss::set_slide_knockback(float value) {
    slide_knockback = value;
}

float MiniBoss::get_slide_knockback() const {
    return slide_knockback;
}
