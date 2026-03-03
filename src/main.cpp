#include <bn_core.h>
#include <bn_display.h>
#include <bn_size.h>
#include <bn_random.h>
#include <bn_backdrop.h>
#include <bn_color.h>

#include "common_fixed_8x16_font.h"
#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include "ScoreDisplay.h"
#include "Player.h"
#include "Enemy.h"

// Width and height of the the player bounding box
static constexpr bn::size PLAYER_SIZE = {3, 3};
static constexpr bn::size ENEMY_SIZE = {17, 17};

static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

int main()
{
    bn::core::init();

    bn::backdrop::set_color(bn::color(0, 0, 10));


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

        for(Enemy& e : enemies)
        {
            if(e.bounding_box.intersects(player.bounding_box))
            {
                player.sprite.set_x(-50);
                player.sprite.set_y(22);
                scoreDisplay.resetScore();
            }
        }

        // Update and display scores after checking all enemies
        scoreDisplay.update();

        // Refresh the screen
        bn::core::update();
    }
}