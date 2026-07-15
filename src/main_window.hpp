#ifndef MAIN_WINDOW_INCLUDE_HPP_
#define MAIN_WINDOW_INCLUDE_HPP_

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/world_boundary_shape2d.hpp>
#include <godot_cpp/classes/camera2d.hpp>

#include "defs_consnts.hpp"

namespace godot
{

class MainWindow : public Node
{
    GDCLASS(MainWindow, Node)

    private:
        Vector2i default_size;
        Vector2i last_size;

        Vector2i pet_position;
        Window* main_window;
        Camera2D* main_camera;
        ProjectSettings* project_settings;
        DisplayServer* ds;
        CharacterBody2D* pet;
        CollisionShape2D* pet_col;
        StaticBody2D* boundaries;

        Vector2i left_top_corner;
        Vector2i right_bottom_corner;

        CollisionShape2D* floor;
        Ref<WorldBoundaryShape2D> wb_floor;

        CollisionShape2D* ceiling;
        Ref<WorldBoundaryShape2D> wb_ceiling;

        CollisionShape2D* left_wall;
        Ref<WorldBoundaryShape2D> wb_left_wall;

        CollisionShape2D* right_wall;
        Ref<WorldBoundaryShape2D> wb_right_wall;

    protected:
        static void _bind_methods();

    public:
        MainWindow();
        ~MainWindow();

        void _ready() override;
        void _physics_process(double delta) override;
};

} // godot

#endif // MAIN_WINDOW_INCLUDE_HPP_
