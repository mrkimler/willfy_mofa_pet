#include "pet_class.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Pet::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_speed"), &Pet::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Pet::set_speed);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

	ClassDB::bind_method(D_METHOD("get_crawl_speed"), &Pet::get_crawl_speed);
	ClassDB::bind_method(D_METHOD("set_crawl_speed", "p_crawl_speed"), &Pet::set_crawl_speed);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "crawl_speed"), "set_crawl_speed", "get_crawl_speed");
}

Pet::Pet()
{
	drag_offset = Vector2(0.f, 0.f);
	is_dragging = false;
	_first_enter_state = false;
	enter_counter = 0;
    _pet_state = pet_state::WALKING;
    _pet_crawling = pet_crawling::NO_CRAWLING;
    _enter_state = false;
	speed = 300.f;
	crawl_speed = 100.f;
}

Pet::~Pet()
{

}

void Pet::_ready()
{
	set_pickable(true);

    willy_mofa_an = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    willy_mofa_an->set_visible(true);
    willy_mofa_an->play("standing");
    set_global_position(Vector2i(1080/2, 1920/2));

    ray_right = get_node<RayCast2D>("RightSide");
    ray_left = get_node<RayCast2D>("LeftSide");

    input =  Input::get_singleton();

#ifdef TEST
    UtilityFunctions::print("pet position from Pet: ", get_position());
    UtilityFunctions::print("pet global position from Pet: ", get_global_position());
#endif // TEST .
}

void Pet::_input_event(Viewport *p_viewport, const Ref<InputEvent> &p_event, int32_t p_shape_idx)
{
	if(p_event->is_class("InputEventMouseButton"))
	{
		Ref<InputEventMouseButton> msb = p_event;
		if(msb->is_pressed() && (msb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT))
		{
			is_dragging = true;
		}
	}
}

void Pet::_input(const Ref<InputEvent> &event)
{
	if(event->is_class("InputEventMouseButton"))
	{
		Ref<InputEventMouseButton> msb = event;

		if(msb->is_released() && msb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
		{
			is_dragging = false;
		}
	}

	if(event->is_class("InputEventMouseMotion") && is_dragging == true)
	{
		Ref<InputEventMouseMotion> msm = event;
		set_global_position(get_global_position() + msm->get_screen_relative());
	}
}

void Pet::walking() // Velocity X
{
	if(_pet_state == pet_state::WALKING || _pet_state == pet_state::CRAWLING)
	{
    	if (input->is_action_just_pressed("up") && is_on_floor() && _pet_state == pet_state::WALKING)
    	{
     		velocity.y = -350.f;
     	}
		else if (input->is_action_pressed("left") && _pet_crawling != pet_crawling::CRAWLING_LEFT)
		{
			input_direction.x = -1.f;
		}
		else if (input->is_action_pressed("right") && _pet_crawling != pet_crawling::CRAWLING_RIGHT)
		{
			input_direction.x = 1.f;
		}
    	else
    	{
        	input_direction.x = 0.f;
     	}

     	if(input_direction.x != 0.f)
    	{
    		last_input_direction.x = input_direction.x;
     	}

      	velocity.x = input_direction.x*speed;
    }
}

void Pet::enter_state(int counter)
{
	static int _counter = 5;

	if(_enter_state == false)
	{
		if(_pet_crawling == pet_crawling::CRAWLING_LEFT)
		{
			velocity.x = -50.f;
		}
		else if(_pet_crawling == pet_crawling::CRAWLING_RIGHT)
		{
			velocity.x = 50.f;
		}

		if(_first_enter_state == false)
		{
			velocity.x *= 5;

			_first_enter_state = true;
			_enter_state = true;
		}

		if(counter >= _counter)
		{
			_enter_state = true;
		}
	}
}

void Pet::crawling() // Velocity Y
{
	if(_pet_state == pet_state::WALKING || _pet_state == pet_state::CRAWLING)
	{
    	if(ray_right->is_colliding() || ray_left->is_colliding())
    	{
        	_pet_state = pet_state::CRAWLING;

         	if(ray_left->is_colliding())
        	{
            	_pet_crawling = pet_crawling::CRAWLING_LEFT;
         	}
        	else
        	{
            	_pet_crawling = pet_crawling::CRAWLING_RIGHT;
         	}

          	enter_state(enter_counter);

           	if(enter_counter < 11)
        	{
        		++enter_counter;
         	}

            if(input->is_action_pressed("up"))
        	{
            	input_direction.y = -1.f;
         	}
        	else if(input->is_action_pressed("down"))
        	{
            	input_direction.y = 1.f;
         	}
        	else
        	{
            	input_direction.y = 0.f;
         	}

            velocity.y = input_direction.y*crawl_speed;
     	}
    	else
    	{
    		enter_counter = 0;
      		_enter_state = false;
        	_pet_state = pet_state::WALKING;
         	_pet_crawling = pet_crawling::NO_CRAWLING;
     	}
    }
}

void Pet::dragging()
{
	if(is_dragging == true)
	{
		_pet_state = pet_state::DRAGGING;
		velocity = Vector2(0.f, 0.f);
	}
	else if(is_dragging == false && _pet_state != pet_state::CRAWLING)
	{
		_pet_state = pet_state::WALKING;
	}

#ifdef TEST
	UtilityFunctions::print("Mouse position: ", get_window()->get_mouse_position());
	UtilityFunctions::print("Mouse global position: ", get_global_mouse_position());
	UtilityFunctions::print("Pet global position: ", get_global_position());
	UtilityFunctions::print("Pet relative position: ", get_position());
	UtilityFunctions::print("Length of vector between get_mouse and pet position: ", (get_window()->get_mouse_position() - get_global_position()).length());
	UtilityFunctions::print("Length of size vector of standing texture: ", ((willy_mofa_an->get_sprite_frames()->get_frame_texture("standing", 1)->get_size()+Vector2(1.f, 1.f)) * get_scale()).length());
#endif //TEST
}

void Pet::animation(double *delta)
{
    const double zero_velocity_y = (velocity.y - (gravity.y*(*delta)));

    if(_pet_state == pet_state::WALKING)
    {
    	if(velocity.x < 0.f || last_input_direction.x < 0.f)
     	{
      		willy_mofa_an->set_flip_h(true);
      	}
     	else
      	{
       		willy_mofa_an->set_flip_h(false);
      	}

        if(!Math::is_zero_approx(zero_velocity_y) && !is_on_floor())
        {
           willy_mofa_an->play("jump");
        }
        else if(!Math::is_zero_approx(velocity.x))
        {
	        willy_mofa_an->play("walking");
        }
        else if(Math::is_zero_approx(zero_velocity_y) && Math::is_zero_approx(velocity.x))
        {
           willy_mofa_an->play("standing");
        }
        else
        {
           willy_mofa_an->stop();
        }
    }
    else if(_pet_state == pet_state::CRAWLING)
    {
    	if(_pet_crawling == pet_crawling::CRAWLING_LEFT)
     	{
      		willy_mofa_an->set_flip_h(true);
      	}
     	else
      	{
       		willy_mofa_an->set_flip_h(false);
      	}

    	willy_mofa_an->play("crawling");
    }
}

void Pet::pet_ai(double *delta)
{
	static int counter = 0;

	++counter;
}

void Pet::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    velocity = get_velocity();

    gravity = get_gravity();//ProjectSettings::get_singleton()->get_setting("physics/2d/default_gravity");
	velocity.y += gravity.y * delta;

	walking();
	crawling();
	dragging();
	animation(&delta);
	pet_ai(&delta);

	set_velocity(velocity);
	move_and_slide();
}

void Pet::set_speed(const float p_speed)
{
	speed = p_speed;
}

float Pet::get_speed() const
{
	return speed;
}

void Pet::set_crawl_speed(const float p_crawl_speed)
{
	crawl_speed = p_crawl_speed;
}

float Pet::get_crawl_speed() const
{
	return crawl_speed;
}
