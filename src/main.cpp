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

// Width and height of the the player bounding box
static constexpr bn::size PLAYER_SIZE = {3, 3};
static constexpr bn::size ENEMY_SIZE = {17, 17};

static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

// Number of characters required to show two of the longest numer possible in an int (-2147483647)
static constexpr int MAX_SCORE_CHARS = 22;

// Score location
static constexpr int SCORE_X = 70;
static constexpr int SCORE_Y = -70;

// High score location
static constexpr int HIGH_SCORE_X = -70;
static constexpr int HIGH_SCORE_Y = -70;

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
/**
 * Displays a score and high score.
 *
 * Score starts a 0 and is increased each time update is called, and reset to 0 when resetScore is
 * called. High score tracks the highest score ever reached.
 */
// class ScoreDisplay
// {
// public:
//     ScoreDisplay() : score(0),                                                                 // Start score at 0
//                      high_score(0),                                                            // Start high score at 0
//                      score_sprites(bn::vector<bn::sprite_ptr, MAX_SCORE_CHARS>()),             // Start with empty vector for score sprites
//                      text_generator(bn::sprite_text_generator(common::fixed_8x16_sprite_font)) // Use a new text generator
//     {
//     }

//     /**
//      * Increases score by 1 and updates high score if needed. Displays score and high score.
//      */
//     void update()
//     {
//         // increase score and update high score if this is the new highest
//         score++;
//         if (score > high_score)
//         {
//             high_score = score;
//         }

//         // Stop displaying previous scores
//         score_sprites.clear();

//         // Display new scores
//         show_number(SCORE_X, SCORE_Y, score);
//         show_number(HIGH_SCORE_X, HIGH_SCORE_Y, high_score);
//     }

//     /**
//      * Displays a number at the given position
//      */
//     void show_number(int x, int y, int number)
//     {
//         // Convert number to a string and then display it
//         bn::string<MAX_SCORE_CHARS> number_string = bn::to_string<MAX_SCORE_CHARS>(number);
//         text_generator.generate(x, y, number_string, score_sprites);
//     }

//     /**
//      * Sets score back to 0. Does NOT reset high score.
//      */
//     void resetScore()
//     {
//         score = 0;
//     }

//     int score;                                                 // current score
//     int high_score;                                            // best core
//     bn::vector<bn::sprite_ptr, MAX_SCORE_CHARS> score_sprites; // Sprites to display scores
//     bn::sprite_text_generator text_generator;                  // Text generator for scores
// };

class Player
{
public:
    Player(int starting_x, int starting_y, bn::fixed player_speed, bn::size player_size) : sprite(bn::sprite_items::dot.create_sprite(starting_x, starting_y)),
                                                                                           speed(player_speed),
                                                                                           size(player_size),
                                                                                           bounding_box(create_bounding_box(sprite, size))
    {
    }

    /**
     * Update the position and bounding box of the player based on d-pad movement.
     */
    void update()
    {
        if (bn::keypad::right_held())
        {
            sprite.set_x(sprite.x() + speed);
        }
        if (bn::keypad::left_held())
        {
            sprite.set_x(sprite.x() - speed);
        }
        if (bn::keypad::up_held())
        {
            sprite.set_y(sprite.y() - speed);
        }
        if (bn::keypad::down_held())
        {
            sprite.set_y(sprite.y() + speed);
        }

        // Prevent going off screen
        int half_width = size.width() / 2;
        int half_height = size.height() / 2;

        if (sprite.x() < MIN_X + half_width)
        {
            sprite.set_x(MIN_X + half_width);
        }
        if (sprite.x() > MAX_X - half_width)
        {
            sprite.set_x(MAX_X - half_width);
        }
        if (sprite.y() < MIN_Y + half_height)
        {
            sprite.set_y(MIN_Y + half_height);
        }
        if (sprite.y() > MAX_Y - half_height)
        {
            sprite.set_y(MAX_Y - half_height);
        }

        bounding_box = create_bounding_box(sprite, size);
    }

    // Create the sprite. This will be moved to a constructor
    bn::sprite_ptr sprite;
    bn::fixed speed;       // The speed of the player
    bn::size size;         // The width and height of the sprite
    bn::rect bounding_box; // The rectangle around the sprite for checking collision
};

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