#include "register_types.h"

#include <initializer_list>

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include "example_class.h"
#include "main_window.hpp"
#include "mouse_dragging.hpp"
#include "pet_class.hpp"
#include "floor_class.hpp"

using namespace godot;

void init_new_action(InputMap *input_map, const StringName p_action, const std::initializer_list<Key> keys)
{
    for(Key key : keys)
    {
        Ref<InputEventKey> key_event = memnew(InputEventKey());
        key_event->set_keycode(key);
        key_event->set_pressed(true);
        input_map->action_add_event(p_action, key_event);
    }
}

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	InputMap* input_map = InputMap::get_singleton();

	Ref<InputEventKey> key_event = memnew(InputEventKey);

	input_map->add_action("left", 0.2f);
	input_map->add_action("right", 0.2f);
	input_map->add_action("up", 0.2f);
	input_map->add_action("down", 0.2f);

	init_new_action(input_map, "up", {KEY_W, KEY_UP});
	init_new_action(input_map, "left", {KEY_A, KEY_LEFT});
	init_new_action(input_map, "down", {KEY_S, KEY_DOWN});
	init_new_action(input_map, "right", {KEY_D, KEY_RIGHT});

	GDREGISTER_CLASS(MainWindow);
	GDREGISTER_CLASS(ExampleClass);
	GDREGISTER_CLASS(Floor);
	GDREGISTER_CLASS(Pet);
	GDREGISTER_CLASS(MouseDragging);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT test_lib_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
