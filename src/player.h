#pragma once

#include "actor.h"

namespace godot {

class Player : public Actor {
    GDCLASS(Player, Actor)

private:
    int max_mana = 100;
    int mana = max_mana;

protected:
    static void _bind_methods();

public:
    Player();
    ~Player();

    void _ready() override;
    void _physics_process(double delta) override;
};

}