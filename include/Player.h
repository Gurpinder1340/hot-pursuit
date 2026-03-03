#ifndef PLAYER_H
#define PLAYER_H

#include <bn_fixed.h>
#include <bn_rect.h>
#include <bn_size.h>
#include <bn_sprite_ptr.h>

class Player
{
public:
    Player(int starting_x, int starting_y, bn::fixed player_speed, bn::size player_size);

    void update();

    bn::sprite_ptr sprite;
    bn::rect bounding_box;

private:
    bn::fixed speed;
    bn::size size;
};

#endif