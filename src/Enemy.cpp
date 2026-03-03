#include "Enemy.h"
#include "Player.h"

#include <bn_display.h>

#include "bn_sprite_items_square.h"

static bn::rect create_bounding_box(const bn::sprite_ptr& sprite, bn::size box_size)
{
    return bn::rect(sprite.x().round_integer(),
                    sprite.y().round_integer(),
                    box_size.width(),
                    box_size.height());
}

static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y =  bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X =  bn::display::width() / 2;

Enemy::Enemy(int starting_x, int starting_y, bn::size enemy_size, bn::fixed enemy_speed) :
    sprite(bn::sprite_items::square.create_sprite(starting_x, starting_y)),
    bounding_box(create_bounding_box(sprite, enemy_size)),
    size(enemy_size),
    speed(enemy_speed),
    random()
{
}

void Enemy::_update_bounding_box()
{
    bounding_box = create_bounding_box(sprite, size);
}

void Enemy::update(Player& player)
{
    // Move in X direction toward player
    if(sprite.x() < player.sprite.x())
        sprite.set_x(sprite.x() + speed);
    else if(sprite.x() > player.sprite.x())
        sprite.set_x(sprite.x() - speed);

    // Move in Y direction toward player
    if(sprite.y() < player.sprite.y())
        sprite.set_y(sprite.y() + speed);
    else if(sprite.y() > player.sprite.y())
        sprite.set_y(sprite.y() - speed);

    _update_bounding_box();
}

void Enemy::jump_to_random_position()
{
    int random_x = random.get_int(MIN_X + 10, MAX_X - 10);
    int random_y = random.get_int(MIN_Y + 10, MAX_Y - 10);

    sprite.set_x(random_x);
    sprite.set_y(random_y);

    _update_bounding_box();
}