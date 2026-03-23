#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void place_cherry(s_game *game) {
    bool occupied;
    do {
        // number between 0 .. GRID_SIZE    , example 17
        game->cherry.x = rand() % GRID_SIZE;
        game->cherry.y = rand() % GRID_SIZE;
        occupied = false;

        // collision
        for (int i = 0; i < game->length; ++i) {
            if (game->body[i].x == game->cherry.x && game->body[i].y == game->cherry.y) {
                occupied = true;
                break;
            }
        }
    } while (occupied);
}

// cx - i places segments to the left of the head, consistent with direction = RIGHT
void game_init(s_game *game) {
    srand(time(NULL));

    game->length = 3;
    int cx = GRID_SIZE / 2;
    int cy = GRID_SIZE / 2;

    for (int i = 0; i < game->length; ++i) {
        game->body[i].x = cx - i;
        game->body[i].y = cy;
    }

    game->current_direction = RIGHT;
    game->next_direction = game->current_direction;
    game->score = 0;
    game->state = RUNNING;
    game->last_tick_time = 0.0;
    game->tick_interval = 0.2;
    place_cherry(game);
}

// avoid 180° turn
void game_set_direction(s_game *game, e_game_direction direction) {
    if ((game->current_direction == UP && direction == DOWN) ||
            (game->current_direction == DOWN && direction == UP) ||
            (game->current_direction == LEFT && direction == RIGHT) ||
            (game->current_direction == RIGHT && direction == LEFT))
        return;
    game->next_direction = direction;
}

void game_update(s_game *game, double current_time) {
    if (game->state != RUNNING) return;
    if ((current_time - game->last_tick_time) < (game->tick_interval)) return;

    game->current_direction = game->next_direction;

    s_game_point new_head = game->body[0];

    // based on the current direction we modify the coordinates
    switch (game->current_direction) {
        case UP:
            new_head.y--;
            break;
        case DOWN:
            new_head.y++;
            break;
        case LEFT:
            new_head.x--;
            break;
        case RIGHT:
            new_head.x++;
            break;
    }

    // wall collision (based also on the grid size )
    if (new_head.x < 0 || new_head.x >= GRID_SIZE ||
            new_head.y < 0 || new_head.y >= GRID_SIZE) {
        game->state = GAME_OVER;
        return;
    }

    for (int i = 0; i < game->length; ++i) {
        // self collision
        if (game->body[i].x == new_head.x && game->body[i].y == new_head.y) {
            game->state = GAME_OVER;
            return;
        }
    }

    // we flag this value if we're eating the cherry
    bool ate = (new_head.x == game->cherry.x && new_head.y == game->cherry.y);

    // shift body (if ate, length grows by 1)
    int tail = ate ? game->length : game->length - 1;
    for (int i = tail; i > 0; i--) {
        game->body[i] = game->body[i - 1];
    }
    game->body[0] = new_head;

    if(ate){
        game->length++;
        game->score++;
        place_cherry(game);
    }

    game->last_tick_time = current_time;
}

void game_restart(s_game * game){
    game_init(game);
}

