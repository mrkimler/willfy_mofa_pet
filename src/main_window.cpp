#include "main_window.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/touch_screen_button.hpp>
#include <utility>

using namespace godot;

void MainWindow::_bind_methods()
{

}

MainWindow::MainWindow()
{
    ds = DisplayServer::get_singleton();
    ERR_FAIL_COND_MSG(ds == nullptr, "Display Server is nullptr");
    if(!Engine::get_singleton()->is_editor_hint())
    {
        ds->window_set_mode(ds->WindowMode::WINDOW_MODE_WINDOWED);
    }
    ds->window_set_current_screen(ds->get_keyboard_focus_screen());
    ds->window_set_flag(ds->WindowFlags::WINDOW_FLAG_TRANSPARENT, true);
    project_settings = ProjectSettings::get_singleton();
    ERR_FAIL_COND_MSG(project_settings == nullptr, "Project Settings is nullptr");
    project_settings->set_setting("display/window/per_pixel_transparency/allowed", true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::_ready()
{
    //project_settings->set_setting("rendering/environment/defaults/default_clear_color", Color(0.5, 0, 0.5, 1.f));

    //TouchScreenButton* root_node = get_tree()->get_root()->get_node<TouchScreenButton>("Main");
    //ERR_FAIL_COND_MSG(root_node == nullptr, "Failed to find root node");

    boundaries = memnew(StaticBody2D);
    add_child(boundaries);
    ERR_FAIL_COND_MSG(boundaries == nullptr, "Failed creating boundaries");

    floor = memnew(CollisionShape2D);
    boundaries->add_child(floor);
    wb_floor = Ref<WorldBoundaryShape2D>(memnew(WorldBoundaryShape2D));
    wb_floor->set_normal(Vector2(0, -1));
    floor->set_shape(wb_floor);

    ceiling = memnew(CollisionShape2D);
    boundaries->add_child(ceiling);
    wb_ceiling = Ref<WorldBoundaryShape2D>(memnew(WorldBoundaryShape2D));
    wb_ceiling->set_normal(Vector2(0, 1));
    ceiling->set_shape(wb_ceiling);

    left_wall = memnew(CollisionShape2D);
    boundaries->add_child(left_wall);
    wb_left_wall = Ref<WorldBoundaryShape2D>(memnew(WorldBoundaryShape2D));
    wb_left_wall->set_normal(Vector2(1, 0));
    left_wall->set_shape(wb_left_wall);

    right_wall = memnew(CollisionShape2D);
    boundaries->add_child(right_wall);
    wb_right_wall = Ref<WorldBoundaryShape2D>(memnew(WorldBoundaryShape2D));
    wb_right_wall->set_normal(Vector2(-1, 0));
    right_wall->set_shape(wb_right_wall);

    main_window = get_window();
    ERR_FAIL_COND_MSG(main_window == nullptr, "Window is nullptr");
    main_window->set_current_screen(ds->get_keyboard_focus_screen());

    ds->window_set_exclusive(main_window->get_window_id(), false);

    main_camera = memnew(Camera2D);
    add_child(main_camera);

    pet = get_parent()->get_node<CharacterBody2D>("Pet");
    ERR_FAIL_COND_MSG(pet == nullptr, "Pet is nullptr");

    pet_col = get_owner()->get_node<CollisionShape2D>("Pet/CollisionShape2D");
    ERR_FAIL_COND_MSG(pet_col == nullptr, "Pet CollisionShape2D is nullptr");

    const Rect2i available_zone = ds->screen_get_usable_rect(DisplayServer::SCREEN_OF_MAIN_WINDOW);
    left_top_corner = available_zone.get_position();
    right_bottom_corner = available_zone.get_end();

    floor->set_position((Vector2) right_bottom_corner);
    ceiling->set_position((Vector2) left_top_corner);
    left_wall->set_position((Vector2) left_top_corner);
    right_wall->set_position((Vector2) right_bottom_corner);

    if(pet != nullptr)
    {
        AnimatedSprite2D* pet_anim = get_owner()->get_node<AnimatedSprite2D>("Pet/AnimatedSprite2D");
        ERR_FAIL_COND_MSG(pet_anim == nullptr, "Pet animation is nullptr");
        Ref<SpriteFrames> pet_frames = pet_anim->get_sprite_frames();
        ERR_FAIL_COND_MSG(pet_frames.is_null(), "Standing second frame don't exist");
        ERR_FAIL_COND_MSG(!pet_frames->has_animation("standing"), "Animation standing not found");
        ERR_FAIL_COND_MSG(pet_frames->get_frame_count("standing") <= 1, "Not enough frames in standing");

        Ref<Texture2D> pet_texture = pet_frames->get_frame_texture("standing", 1);
        ERR_FAIL_COND_MSG(pet_texture.is_null(), "Texture for frame 1 is null");

        get_tree()->get_root()->set_transparent_background(true);
        main_window->set_transparent_background(true); //transperent back ground

        main_window->set_embedding_subwindows(false);
        main_window->set_flag(main_window->Flags::FLAG_TRANSPARENT, true); //transperent

        if (!Engine::get_singleton()->is_editor_hint())
        {
            main_window->set_flag(main_window->Flags::FLAG_BORDERLESS, true); //borderless
            main_window->set_flag(main_window->Flags::FLAG_RESIZE_DISABLED, true);
            main_window->set_flag(main_window->Flags::FLAG_ALWAYS_ON_TOP, true);
            main_window->set_min_size(Vector2i((pet_texture->get_size()+Vector2(1.f, 1.f)) * pet->get_scale()));

            main_window->set_size(main_window->get_min_size());

            //default_size = main_window->get_size();
            //last_size = default_size;
           	//main_window->set_canvas_cull_mask_bit(1, true);
            //main_window->set_canvas_cull_mask_bit(0, false);
        }

        main_window->set_initial_position(main_window->WindowInitialPosition::WINDOW_INITIAL_POSITION_ABSOLUTE);

#ifdef TEST
        if (!Engine::get_singleton()->is_editor_hint())
        {
            UtilityFunctions::print("sprite visible: ", pet_anim->is_visible());
            UtilityFunctions::print("sprite playing: ", pet_anim->is_playing());
            UtilityFunctions::print("sprite frame: ", pet_anim->get_frame());
            UtilityFunctions::print("sprite texture size: ", pet_texture->get_size());
            UtilityFunctions::print("window size: ", main_window->get_size());
            UtilityFunctions::print("window position: ", main_window->get_position());
            UtilityFunctions::print("pet global pos from main_window: ", pet->get_global_position());
            UtilityFunctions::print("pet position from main_window: ", pet->get_position());
            UtilityFunctions::print("main_camera position: ", main_camera->get_position());
        }
#endif // TEST
    }
}

void MainWindow::_physics_process(double delta)
{
    /*if (Engine::get_singleton()->is_editor_hint())
    {
        return;
        }*/
    if (!Engine::get_singleton()->is_editor_hint())
    {
        if(pet != nullptr && pet_col != nullptr)
        {
            pet_position = pet->get_global_position();
            const Vector2i wright_bottom = (main_window->get_position() + main_window->get_size());
            const Vector2i wleft_top = main_window->get_position();
            const Ref<RectangleShape2D> pet_sh_rect = pet_col->get_shape();
            const Vector2i psr_offset = Vector2i(pet_col->get_position());
            const Vector2i pr_top_left = pet_position - pet_sh_rect->get_size()/2 + psr_offset;
            const Vector2i pr_bottom_right = pet_position + pet_sh_rect->get_size()/2 + psr_offset;

            /*
            const PackedVector2Array col_corners = {
            	Vector2(pr_top_left - main_window->get_position()),
            	Vector2(pr_bottom_right.x - main_window->get_position().x, pr_top_left.y - main_window->get_position().y),
             	Vector2(pr_top_left.x - main_window->get_position().x, pr_bottom_right.y - main_window->get_position().y),
             	Vector2(pr_bottom_right)
            };

            ds->window_set_mouse_passthrough(col_corners);

            if(pet_position.x >= (right_bottom_corner.x - main_window->get_size().x))
            {
                UtilityFunctions::print("TRUE!!!!!!");
                main_window->set_position(right_bottom_corner);
            }
            else
            {*/
            main_window->set_position(pet_position - (main_window->get_size()/2));
            //}
#ifdef TEST
            UtilityFunctions::print("Pet shape rectangle right side X: ", pr_bottom_right.x);
            UtilityFunctions::print("Pet shape rectangle left side X: ", pr_top_left.x);
            UtilityFunctions::print("Main Window rectangle right side X: ", wright_bottom.x);
            UtilityFunctions::print("Main Window rectangle left side X: ", wleft_top.x);
            UtilityFunctions::print("Main Window last size: ", last_size);
            UtilityFunctions::print("Left top corner of sceen: ", left_top_corner);
            UtilityFunctions::print("Right bottom corner of sceen: ", right_bottom_corner);
            UtilityFunctions::print("Pet global pos from main_window: ", pet->get_global_position());
            UtilityFunctions::print("Pet collision offset: ", psr_offset);
#endif //TEST

            main_camera->set_position(pet_position);
        }
    }
}
