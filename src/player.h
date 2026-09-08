#pragma once

#include "actor.h"

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:
    float move_speed = 200.0f;

protected:
    static void _bind_methods();

public:
    Player();
    ~Player();

    void _ready() override;
    void _physics_process(double delta) override;
};

}