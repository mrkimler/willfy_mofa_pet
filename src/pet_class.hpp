#ifndef PET_INCLUDE_HPP_
#define PET_INCLUDE_HPP_

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

#include "defs_consnts.hpp"

#define GRAVITY_CONST 98.f*400

namespace godot
{

class Pet : public CharacterBody2D
{
	GDCLASS(Pet, CharacterBody2D)

	private:
		int enter_counter;

		float speed;
		float crawl_speed;

		Vector2 gravity;
		Vector2 velocity;
		Vector2 drag_velocity;
		Vector2 input_direction;
		Vector2 last_input_direction;
		Vector2 drag_offset;

		AnimatedSprite2D* willy_mofa_an;
		RayCast2D* ray_right;
		RayCast2D* ray_left;

		Input* input;
		RandomNumberGenerator* rng;

		int _pet_state;
		int _pet_crawling;
		int _pet_ai;
		int _last_pet_ai;

		bool is_dragging;
		bool just_released;


		enum pet_state
		{
		    WALKING = 0,
		    CRAWLING = 1,
			DRAGGING = 2,
			THROWING = 3
		};

		enum pet_crawling
		{
		    NO_CRAWLING = 0,
            CRAWLING_RIGHT = 1,
            CRAWLING_LEFT = 2
		};

		enum pet_ai
		{
			LEFT,
			RIGHT,
			JUMP,
			STAND,
			UP,
			DOWN
		};

	protected:
		static void _bind_methods();

	public:
		~Pet();
		Pet();

		void crawling();
		void walking();
		void dragging(double *delta);
		void throwing();
		void animation(double *delta);
		void enter_state(int counter);
		void pet_ai(int *frq_delta);

		void _ready() override;

		void _input_event(Viewport *p_viewport, const Ref<InputEvent> &p_event, int32_t p_shape_idx) override;
		void _input(const Ref<InputEvent> &event) override;
		void _physics_process(double delta) override;

		void set_speed(const float p_speed);
		float get_speed() const;

		void set_crawl_speed(const float p_crawl_speed);
		float get_crawl_speed() const;
};

} // godot

#endif // PET_INCLUDE_HPP_
