#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../common_objects/objects.h"

#define TRUE 1
#define FALSE 0

#define FIELD_COLS 10
#define FIELD_ROWS 20

#define BLOCK_SIZE 4

#define HIGH_SCORE_MEM "highest_score.txt"

#define SPEED_START 3000

#define LEVEL_MAX 2
#define LEVEL_STEP 600

#define COST_ONE_LINE 100
#define COST_TWO_LINE 300
#define COST_THREE_LINE 700
#define COST_FOUR_LINE 1500

#define FILL_L(block) \
  {                   \
    block[0][1] = 1;  \
    block[1][1] = 1;  \
    block[2][1] = 1;  \
    block[2][2] = 1;  \
  }
#define FILL_J(block) \
  {                   \
    block[0][1] = 2;  \
    block[1][1] = 2;  \
    block[2][1] = 2;  \
    block[2][0] = 2;  \
  }

#define FILL_S(block) \
  {                   \
    block[1][1] = 3;  \
    block[1][2] = 3;  \
    block[2][1] = 3;  \
    block[2][0] = 3;  \
  }

#define FILL_Z(block) \
  {                   \
    block[1][0] = 4;  \
    block[1][1] = 4;  \
    block[2][1] = 4;  \
    block[2][2] = 4;  \
  }
#define FILL_I(block) \
  {                   \
    block[1][0] = 5;  \
    block[1][1] = 5;  \
    block[1][2] = 5;  \
    block[1][3] = 5;  \
  }
#define FILL_T(block) \
  {                   \
    block[1][0] = 6;  \
    block[1][1] = 6;  \
    block[1][2] = 6;  \
    block[0][1] = 6;  \
  }
#define FILL_O(block) \
  {                   \
    block[1][1] = 7;  \
    block[1][2] = 7;  \
    block[2][1] = 7;  \
    block[2][2] = 7;  \
  }

typedef enum {
  START = 0,
  SPAWN,
  ACTION,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE
} State_t;

typedef struct {
  State_t status;
  int pause;

  int** field;

  int next_name;
  int** next;
  int block_name;
  int** block;
  int block_x;
  int block_y;
  int rotate;

  int score;
  int high_score;
  int level;
  int speed;

  unsigned long long start_time;
} Game_t;

typedef enum {
  FIG_J = 0,
  FIG_L,
  FIG_O,
  FIG_I,
  FIG_Z,
  FIG_S,
  FIG_T
} Block_name_t;

// вход в библиотеку
void userInput(UserAction_t action, int hold);

// выход из библиотеки
GameInfo_t updateCurrentState();

// работа с рекордом
void get_hight_score();
void set_hight_score();
void update_score(int count);
void update_level();

// работа со статусом
void game_init();
void game_end();
void block_spawn();
void block_moving(UserAction_t act);
void block_attaching();
void check_overflow();

// работа с матрицами
int** create_matrix(int rows, int cols);
void remove_matrix(int** matrix, int rows);
int check_row();
void delete_row(int row);
int** copy_matrix(int** src, int size);
void rotate_matrix();
void pin_block();
void unpin_block();
void fill_block(int** block, int name);

// движение блока
void check_time();
unsigned long long get_time();
void shift_left();
void shift_right();
void fall_down();
void shift_down();
void rotate();
int check_attached();
void rotation_prepare(int* is_shift_right, int* is_shift_left);

// костыль для теста
Game_t* game_data();

#endif