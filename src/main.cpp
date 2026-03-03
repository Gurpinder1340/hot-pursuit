#include <bn_core.h>
#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_rect.h>
#include <bn_size.h>
#include <bn_string.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <cstdlib>
#include <ctime>

#include "common_fixed_8x16_font.h"
#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include "ScoreDisplay.h"
#include "Player.h"

// Width and height of the the player bounding box
static constexpr bn::size PLAYER_SIZE = {3, 3};
static constexpr bn::size ENEMY_SIZE = {17, 17};

static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

// Number of characters required to show two of the longest numer possible in an int (-2147483647)
// static constexpr int MAX_SCORE_CHARS = 22;

// // Score location
// static constexpr int SCORE_X = 70;
// static constexpr int SCORE_Y = -70;

// // High score location
// static constexpr int HIGH_SCORE_X = -70;
// static constexpr int HIGH_SCORE_Y = -70;

/**
 * Creates a rectangle centered at a sprite's location with a given size.
 * sprite the sprite to center the box around
 * box_size the dimensions of the bounding box
 */
bn::rect create_bounding_box(bn::sprite_ptr sprite, bn::size box_size)
{
    return bn::rect(sprite.x().round_integer(),
                    sprite.y().round_integer(),
                    box_size.width(),
                    box_size.height());
}


class Enemy
{
public:
    Enemy(int starting_x, int starting_y, bn::size enemy_size, bn::fixed enemy_speed) : sprite(bn::sprite_items::square.create_sprite(starting_x, starting_y)),
                                                                                        size(enemy_size),
                                                                                        bounding_box(create_bounding_box(sprite, size)),
                                                                                        speed(enemy_speed)
    {
    }

    void update(Player &player)
    {
        // Move in X direction toward player
        if (sprite.x() < player.sprite.x())
        {
            sprite.set_x(sprite.x() + speed);
        }
        else if (sprite.x() > player.sprite.x())
        {
            sprite.set_x(sprite.x() - speed);
        }

        // Move in Y direction toward player
        if (sprite.y() < player.sprite.y())
        {
            sprite.set_y(sprite.y() + speed);
        }
        else if (sprite.y() > player.sprite.y())
        {
            sprite.set_y(sprite.y() - speed);
        }

        // Update bounding box
        bounding_box = create_bounding_box(sprite, size);
    }

    // function to jump to a random position
    void jump_to_random_position()
    {
        int random_x = random.get_int(MIN_X + 10, MAX_X - 10);
        int random_y = random.get_int(MIN_Y + 10, MAX_Y - 10);

        sprite.set_x(random_x);
        sprite.set_y(random_y);

        bounding_box = create_bounding_box(sprite, size);
    }

    bn::sprite_ptr sprite;
    bn::size size;
    bn::rect bounding_box;
    bn::fixed speed;
    bn::random random;
};

int main()
{
    bn::core::init();

    // Create a new score display
    ScoreDisplay scoreDisplay;

    // Create a player and initialize it
    Player player = Player(0, 2, 3, PLAYER_SIZE);

    Enemy enemy(40, -10, ENEMY_SIZE, 1.5);

    // Vector of enemies
    bn::vector<Enemy, 10> enemies;
    enemies.push_back(Enemy(-40, 0, ENEMY_SIZE, 1.5));

    // counter to add enemies
    int frame_counter = 0;
    const int FRAMES_TO_ADD = 150;

    // Added enemies
    // enemies.push_back(Enemy(40, -20, ENEMY_SIZE, 1.5));
    // enemies.push_back(Enemy(40,  20, ENEMY_SIZE, 1.5));
    // enemies.push_back(Enemy(10,  20, ENEMY_SIZE, 1.5));
    // enemies.push_back(Enemy(-50, 0,  ENEMY_SIZE, 1.5));

    while (true)
    {
        // Update player position
        player.update();
        enemy.update(player);

        frame_counter++;
        if (frame_counter >= FRAMES_TO_ADD && enemies.size() < 10)
        {
            int random_x = bn::random().get_int(MIN_X + 10, MAX_X - 10);
            int random_y = bn::random().get_int(MIN_Y + 10, MAX_Y - 10);
            enemies.push_back(Enemy(random_x, random_y, ENEMY_SIZE, 1.5));
            frame_counter = 0;
        }

        // Reset the current score and player position if the player collides with enemy
        if (enemy.bounding_box.intersects(player.bounding_box))
        {
            scoreDisplay.resetScore();
            player.sprite.set_x(44);
            player.sprite.set_y(22);
            enemy.jump_to_random_position();
        }

        // Loop over all the enemies
        for (Enemy &e : enemies)
        {
            // Update enemy bounding box
            enemy.bounding_box = create_bounding_box(enemy.sprite, enemy.size);

            // Check for collision with player
            if (e.bounding_box.intersects(player.bounding_box))
            {
                // Reset player position
                player.sprite.set_x(-50);
                player.sprite.set_y(22);

                // Reset score
                scoreDisplay.resetScore();
            }
        }

        // Update and display scores after checking all enemies
        scoreDisplay.update();

        // Refresh the screen
        bn::core::update();
    }
}