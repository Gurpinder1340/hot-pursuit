#ifndef ENEMY_H
#define ENEMY_H

#include <bn_fixed.h>
#include <bn_random.h>
#include <bn_rect.h>
#include <bn_size.h>
#include <bn_sprite_ptr.h>

class Player;

class Enemy
{
public:
    Enemy(int starting_x, int starting_y, bn::size enemy_size, bn::fixed enemy_speed);

    void update(Player& player);
    void jump_to_random_position();

    bn::sprite_ptr sprite;
    bn::rect bounding_box;

private:
    bn::size size;
    bn::fixed speed;
    bn::random random;

    void _update_bounding_box();
};

#endif