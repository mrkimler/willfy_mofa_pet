#ifndef FLOOR_CLASS_INCLUDE_HPP_
#define FLOOR_CLASS_INCLUDE_HPP_

#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/static_body2d.hpp>

namespace godot
{

class Floor : public StaticBody2D //Sprite2D //RectangleShape2D
{
    GDCLASS(Floor, StaticBody2D)

    private:
        Vector2 size;
        CollisionShape2D* shape;
        Ref<RectangleShape2D> rectangle_col;
        void _resize_collision();
        Sprite2D* sprite;

    protected:
        static void _bind_methods();

    public:
        Floor();
        ~Floor();

        void set_size(Vector2 p_size);
        Vector2 get_size();

        void set_my_texture(const Ref<Texture2D> &p_texture);
        Ref<Texture2D> get_my_texture() const;

        Callable get_resize_collision();
};

} // godot

#endif // FLOOR_CLASS_INCLUDE_HPP_
