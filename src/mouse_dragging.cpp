#include "defs_consnts.hpp"
#include "mouse_dragging.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void MouseDragging::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_sp_texture"), &MouseDragging::get_sp_texture);
    ClassDB::bind_method(D_METHOD("set_sp_texture", "p_sprite_texture"), &MouseDragging::set_sp_texture);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_sp_texture", "get_sp_texture");
}

void MouseDragging::_input_event(Viewport *p_viewport, const Ref<InputEvent> &p_event, int32_t p_shape_idx)
{
	if(p_event->is_class("InputEventMouseButton"))
	{
		Ref<InputEventMouseButton> shit = p_event;
		if((shit->is_pressed() && (shit->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT))) /*((get_global_mouse_position() - get_global_position()).length() <= sprite_tex->get_size().length())*/
		{
			dragging = true;
		}
	}
}

void MouseDragging::_input(const Ref<InputEvent> &event)
{
	if(event->is_class("InputEventMouseButton"))
	{
		Ref<InputEventMouseButton> shit = event;
		if(shit->is_released() && shit->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
		{
			dragging = false;
		}
	}

	if(event->is_class("InputEventMouseMotion") && dragging == true)
	{
		Ref<InputEventMouseMotion> shit = event;
		set_global_position(get_global_position() + shit->get_screen_relative());

		last_frame_event = true;
	}
	else if(event->is_class("InputEventMouseMotion") && last_frame_event == true)
	{
		Ref<InputEventMouseMotion> shit = event;
		last_frame_event = false;
		velocity = shit->get_screen_relative() * 10;

		if(velocity.abs() >= Vector2(580.f*2, 580.f*2))
		{
			if(velocity.x > 0.f)
			{

			}

			velocity = Vector2(580.f*2, 580.f*2);
		}

		hehehe = true;
	}
}

MouseDragging::MouseDragging()
{
	dragging = false;
	last_frame_event = false;
	hehehe = false;

	velocity = Vector2(0.f, 0.f);

	sprite = memnew(Sprite2D);
	add_child(sprite);

	sprite_tex = load_rs->load("res://icon.svg");
	sprite->set_texture(sprite_tex);

	sprite_col = memnew(CollisionShape2D);
	add_child(sprite_col);

	sprite_rsh = Ref<RectangleShape2D>(memnew(RectangleShape2D));
	sprite_rsh->set_size(Vector2(100.f, 100.f));
	sprite_col->set_shape(sprite_rsh);

	set_position(Vector2(500.f, 499.f));
}

MouseDragging::~MouseDragging()
{

}

void MouseDragging::_ready()
{
	set_pickable(true);
}

void MouseDragging::_physics_process(double delta)
{
	if(hehehe == false)
	{
		velocity = get_velocity();
	}
	else
	{
		if(!Math::is_zero_approx(velocity.x) || !Math::is_zero_approx(velocity.y))
		{
			if(velocity.x < 0.f)
			{
				velocity.x += 300.f*delta;
			}
			else
			{
				velocity.x -= 300.f*delta;
			}

			if(velocity.y < 0.f)
			{
				velocity.y += 300.f*delta;
			}
			else
			{
				velocity.y -= 300.f*delta;
			}
		}
		else
		{
			hehehe = false;
		}
	}

//	UtilityFunctions::print("True?: ", sprite->get_region_rect().has_point(get_global_mouse_position() - get_global_position()));
//	UtilityFunctions::print("Rect: ", sprite->get_region_rect());
//	UtilityFunctions::print("Distance: ", get_global_mouse_position() - get_global_position());
//	UtilityFunctions::print("Texture size: ", sprite_tex->get_size());

	set_velocity(velocity);
	move_and_slide();
}

void MouseDragging::set_sp_texture(Ref<Texture2D> p_sprite_tex)
{
	sprite_tex = p_sprite_tex;
	sprite->set_texture(sprite_tex);
}

Ref<Texture2D> MouseDragging::get_sp_texture() const
{
	return sprite_tex;
}
