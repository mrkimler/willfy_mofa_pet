#include "floor_class.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Floor::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_size"), &Floor::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "p_size"), &Floor::set_size);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");

    ClassDB::bind_method(D_METHOD("get_my_texture"), &Floor::get_my_texture);
    ClassDB::bind_method(D_METHOD("set_my_texture", "p_texture"), &Floor::set_my_texture);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_my_texture", "get_my_texture");

	ClassDB::bind_method(D_METHOD("get_resize_collision"), &Floor::get_resize_collision);

	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "get_resize_collision", PROPERTY_HINT_TOOL_BUTTON, "resize collision"), "", "get_resize_collision");
}

Floor::Floor()
{
    sprite = memnew(Sprite2D);
    add_child(sprite);

    size.x = 20;
    size.y = 20;

    shape = memnew(CollisionShape2D);
    add_child(shape);
    rectangle_col = Ref<RectangleShape2D>(memnew(RectangleShape2D));
    rectangle_col->set_size(size);
    shape->set_shape(rectangle_col);
}

Floor::~Floor()
{

}

Callable Floor::get_resize_collision()
{
    return callable_mp(this, &Floor::_resize_collision);
}

void Floor::_resize_collision()
{
    Ref<Texture2D> tex = sprite->get_texture();
    if(tex.is_valid())
    {
        size = tex->get_size();

        rectangle_col->set_size(size);
        shape->set_shape(rectangle_col);
    }
}

void Floor::set_size(Vector2 p_size)
{
    size = p_size;
    rectangle_col->set_size(size);
    shape->set_shape(rectangle_col);
}

Vector2 Floor::get_size()
{
    return size;
}

void Floor::set_my_texture(const Ref<Texture2D> &p_texture)
{
    sprite->set_texture(p_texture);
    _resize_collision();
}

Ref<Texture2D> Floor::get_my_texture() const
{
    return sprite->get_texture();
}
