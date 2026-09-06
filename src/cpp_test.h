#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class CppTest : public Node {
    GDCLASS(CppTest, Node)

protected:
    static void _bind_methods();

public:
    void _ready() override;
};

}
