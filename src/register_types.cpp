#include "register_types.h"
#include "actor.h"
#include "player.h"
//#include "enemy.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_projectp_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    GDREGISTER_CLASS(Actor);
    GDREGISTER_CLASS(Player);
    //GDREGISTER_CLASS(Enemy);
}

void uninitialize_projectp_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {

GDExtensionBool GDE_EXPORT projectp_library_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization
) {
    GDExtensionBinding::InitObject init_obj(
        p_get_proc_address,
        p_library,
        r_initialization
    );

    init_obj.register_initializer(initialize_projectp_module);
    init_obj.register_terminator(uninitialize_projectp_module);

    init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_SCENE
    );

    return init_obj.init();
}

}