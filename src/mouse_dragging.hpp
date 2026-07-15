#pragma once

#include <godot_cpp/classes/character_body2d.hpp>

#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/viewport.hpp>

namespace godot
{

class MouseDragging : public CharacterBody2D
{
	GDCLASS(MouseDragging, CharacterBody2D)

	private:
		Ref<Texture2D> sprite_tex;
		Ref<RectangleShape2D> sprite_rsh;
		ResourceLoader* load_rs = ResourceLoader::get_singleton();
		CollisionShape2D* sprite_col;
		Sprite2D* sprite;

		Vector2 velocity;

		bool dragging;
		bool last_frame_event;
		bool hehehe;

	protected:
		static void _bind_methods();

	public:
		MouseDragging();
		~MouseDragging();

		void _input_event(Viewport *p_viewport, const Ref<InputEvent> &p_event, int32_t p_shape_idx) override;
		void _input(const Ref<InputEvent> &event) override;
		void _ready() override;

		void _physics_process(double delta) override;

		void set_sp_texture(const Ref<Texture2D> p_sprite_tex);
		Ref<Texture2D> get_sp_texture() const;
};

} // godot
