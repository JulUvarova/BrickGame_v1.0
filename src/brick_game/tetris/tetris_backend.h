#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdlib.h>

#include "defines.h"
#include "objects.h"

// работа с рекордом
int get_hight_score();
void set_hight_score();
void update_score(int count);
void update_level();

// работа со статусом
void game_init();
void game_end();
void block_spawn();
void block_moving(UserAction_t act);
void check_attaching();
void check_overflow();

// работа с матрицами
int** create_matrix(int rows, int cols);
void remove_matrix(int** matrix, int rows);
int check_row();

int** rotate_matrix(int** old, int rows, int cols);
void shift_left();
void shift_right();
void fall_down();
void shift_down();
void rotate();

int check_collizion();

void userInput(UserAction_t action, bool);
GameInfo_t updateCurrentState();

void game_init() {
  game.status = SPAWN;
  game.pause = FALSE;

  game.field = create_matrix(BOARD_N, BOARD_M);

  game.score = 0;
  game.high_score = get_hight_score();
  game.level = 1;
  game.speed = SPEED;

  game.start_time = 0;
  game.left_time = 0;
}

void game_end() {
  remove_matrix(game.field, BOARD_N);
  remove_matrix(game.next, 4);   // ! fake
  remove_matrix(game.block, 4);  // ! fake
  game.status = EXIT_STATE;
}

void block_spawn() {
  game.next = NULL;
  game.block = NULL;

  if (!game.next || !game.block) {
    game.status = GAMEOVER;
    return;
  }

  game.block_size = 0;
  game.block_x = -1;
  game.block_y = 4;

  // error -> GAMEOVER
  if (check_collizion())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

int** create_matrix(int rows, int cols) {
  int** matrix = calloc(rows, sizeof(int*));
  for (int i = 0; i < rows && matrix; ++i) {
    matrix[i] = calloc(cols / 2, sizeof(int));
    if (matrix[i] == NULL) {
      remove_matrix(matrix, rows);
    }
  }
  if (matrix) matrix[3][3] = '*';
  return matrix;
}

int** rotate_matrix(int** old, int rows, int cols) {
  int** new = create_matrix(rows, cols);
  if (new) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        new[j][i] = old[i][j];
      }
    }
  }
  return new;
}

void remove_matrix(int** matrix, int rows) {
  if (matrix) {
    for (int i = 0; i < rows && matrix[i]; ++i) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

void userInput(UserAction_t act, bool) {
  if (game.pause && act != Terminate && act != Pause) return;
  if (act == Pause) game.pause = (game.pause + 1) % 2;
  if (act == Terminate) game.status = GAMEOVER;

  switch (game.status) {
    case START:
      game_init();
      // error -> GAMEOVER
      break;
    case SPAWN:
      block_spawn();
      // error -> GAMEOVER
      break;
    case ACTION:
      block_moving(act);
      // error -> GAMEOVER
      break;
    case SHIFTING:
      shift_down();
      break;
    case ATTACHING:
      check_attaching();
      break;
    case GAMEOVER:
      game_end();
      break;
    default:
      break;
  }
}

void check_attaching() {
  int count = 0;
  for (int i = 0; i < BOARD_N; ++i) count += check_row();
  update_score(count);
  if (game.score > game.high_score) {
    set_hight_score();
    game.high_score = game.score;
  }
  update_level();
  check_overflow();
}

void check_overflow() {
  for (int i = 0; i < BOARD_M && game.status != GAMEOVER; ++i) {
    if (game.field[0][i]) game.status = GAMEOVER;
  }
}

void update_score(int count) {
  switch (count) {
    case 1:
      game.score += COST_ONE_LINE;
      break;
    case 2:
      game.score += COST_TWO_LINE;
      break;
    case 3:
      game.score += COST_THREE_LINE;
      break;
    case 4:
      game.score += COST_FOUR_LINE;
      break;
    default:
      break;
  }
}

void update_level() {
  game.level = game.score / LEVEL_STEP;
  if (game.level >= LEVEL_MAX) {
    game.level = LEVEL_MAX;
    game.status = GAMEOVER;
  }
}

int check_row() { return 1; }

void block_moving(UserAction_t act) {
  switch (act) {
    case Left:
      shift_left();
      break;
    case Right:
      shift_right();
      break;
    case Down:
      fall_down();
      break;
    case Action:
      rotate();  // matrix
      break;
    default:
      break;
  }
}

int check_collizion() { return FALSE; }

void rotate() {
  if (check_collizion())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

void shift_left() {
  if (check_collizion())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

void shift_right() {
  if (check_collizion())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

void shift_down() {
  if (check_collizion())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

void fall_down() {
  while (game.status != ATTACHING) shift_down();
}

GameInfo_t updateCurrentState() { return game; }

int get_hight_score() {
  int result = 0;
  FILE* file = fopen(HIGH_SCORE_MEM, "r");
  if (file) {
    fscanf(file, "%d", &game.high_score);
  }
  return result;
}

void set_hight_score() {
  FILE* file = fopen(HIGH_SCORE_MEM, "w");
  if (file) {
    fprintf(file, "%d", game.score);
    fclose(file);
  }
}

#endif