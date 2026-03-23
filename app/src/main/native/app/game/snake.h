#pragma once


#ifdef __cplusplus
extern "C" {
#endif

// defining game macros
#define GRID_SIZE 20
#define MAX_SNAKE_LEN (GRID_SIZE * GRID_SIZE)

typedef enum e_game_direction {
    UP, DOWN, LEFT, RIGHT
} e_game_direction;

typedef enum e_game_state {
    RUNNING, GAME_OVER
} e_game_state;

typedef struct s_game_point {
    int x, y;
} s_game_point;

typedef struct s_game {
    s_game_point body[MAX_SNAKE_LEN];
    s_game_point cherry ;
    int length;
    e_game_direction current_direction , next_direction ;

    // not showed to the user
    int score;
    e_game_state state;
    double last_tick_time;
    double tick_interval;
} s_game;

void place_cherry(s_game * game);

void game_init(s_game *game);

void game_set_direction(s_game * game, e_game_direction direction);

void game_update(s_game *game, double current_time);

void game_restart(s_game * game);

#ifdef __cplusplus
}
#endif