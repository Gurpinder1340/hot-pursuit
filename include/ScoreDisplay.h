#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

class ScoreDisplay
{
public:
    ScoreDisplay();

    void update();
    void resetScore();

private:
    void show_number(int x, int y, int number);

    static constexpr int MAX_SCORE_CHARS = 22;
    static constexpr int SCORE_X = 70;
    static constexpr int SCORE_Y = -70;
    static constexpr int HIGH_SCORE_X = -70;
    static constexpr int HIGH_SCORE_Y = -70;

    int score;
    int high_score;
    bn::vector<bn::sprite_ptr, MAX_SCORE_CHARS> score_sprites;
    bn::sprite_text_generator text_generator;
};

#endif