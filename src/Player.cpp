#include "Player.h"

#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_rect.h>

#include "bn_sprite_items_dot.h"

static bn::rect create_bounding_box(const bn::sprite_ptr& sprite, bn::size box_size)
{
    return bn::rect(sprite.x().round_integer(),
                    sprite.y().round_integer(),
                    box_size.width(),
                    box_size.height());
}

Player::Player(int starting_x, int starting_y, bn::fixed player_speed, bn::size player_size) :
    sprite(bn::sprite_items::dot.create_sprite(starting_x, starting_y)),
    bounding_box(create_bounding_box(sprite, size)),
    speed(player_speed),
    size(player_size)
    
{
}


void Player::update()
{
    if(bn::keypad::right_held())
    {
        sprite.set_x(sprite.x() + speed);
    }
    if(bn::keypad::left_held())
    {
        sprite.set_x(sprite.x() - speed);
    }
    if(bn::keypad::up_held())
    {
        sprite.set_y(sprite.y() - speed);
    }
    if(bn::keypad::down_held())
    {
        sprite.set_y(sprite.y() + speed);
    }

    // Screen bounds
    const int min_x = -bn::display::width() / 2;
    const int max_x =  bn::display::width() / 2;
    const int min_y = -bn::display::height() / 2;
    const int max_y =  bn::display::height() / 2;

    const int half_w = size.width() / 2;
    const int half_h = size.height() / 2;

    if(sprite.x() < min_x + half_w)
    {
        sprite.set_x(min_x + half_w);
    }
    if(sprite.x() > max_x - half_w)
    {
        sprite.set_x(max_x - half_w);
    }
    if(sprite.y() < min_y + half_h)
    {
        sprite.set_y(min_y + half_h);
    }
    if(sprite.y() > max_y - half_h)
    {
        sprite.set_y(max_y - half_h);
    }

    bounding_box = create_bounding_box(sprite, size);
}