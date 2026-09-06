#include "cpp_test.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void CppTest::_bind_methods() {
}

void CppTest::_ready() {
    UtilityFunctions::print("C++ is working!");
}
