#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdlib.h>

#include "defines.h"
#include "objects.h"

#define FIELD_COLS 10
#define FIELD_ROWS 20

#define BLOCK_SIZE 4

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
void block_attaching();
void check_overflow();
void pin_block();
void unpin_block();
void fill_block(int** block, int name);

// работа с матрицами
int** create_matrix(int rows, int cols);
void remove_matrix(int** matrix, int rows);
int check_row();
void delete_row(int row);

int** rotate_matrix(int** old, int rows, int cols);
void shift_left();
void shift_right();
void fall_down();
void shift_down();
void rotate();
int check_attached();

void userInput(UserAction_t action, bool);
GameInfo_t updateCurrentState();

void game_init() {
  game.status = SPAWN;
  game.pause = FALSE;

  game.field = create_matrix(FIELD_ROWS, FIELD_COLS);

  game.next_name = rand() % 7;
  game.score = 0;
  game.high_score = get_hight_score();
  game.level = 1;
  game.speed = SPEED_START;

  game.start_time = 0;
  game.left_time = 0;
}

void game_end() {
  // remove_matrix(game.field, FIELD_ROWS);
  // remove_matrix(game.next, BLOCK_SIZE);   // ! fake
  // remove_matrix(game.block, BLOCK_SIZE);  // ! fake
  game.status = EXIT_STATE;
}

void block_spawn() {
  remove_matrix(game.next, BLOCK_SIZE);
  remove_matrix(game.block, BLOCK_SIZE);
  game.block_name = game.next_name;
  game.next_name = rand() % 7;

  game.next = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  game.block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);

  if (!game.next || !game.block) {
    game.status = GAMEOVER;
    return;
  }

  fill_block(game.next, game.next_name);
  fill_block(game.block, game.block_name);

  game.block_y = 0;
  game.block_x = 3;  //! formula
  game.rotate = 1;

  // error -> GAMEOVER
  if (check_attached()) {
    game.status = GAMEOVER;
    game.pause = -1;
  } else {
    pin_block();
    game.status = ACTION;
  }
}

void fill_block(int** block, int name) {
  switch (name) {
    case FIG_L:
      block[0][1] = 1;
      block[1][1] = 1;
      block[2][1] = 1;
      block[2][2] = 1;
      break;
    case FIG_J:
      block[0][1] = 1;
      block[1][1] = 1;
      block[2][1] = 1;
      block[2][0] = 1;
      break;
    case FIG_S:
      block[1][1] = 1;
      block[1][2] = 1;
      block[2][1] = 1;
      block[2][0] = 1;
      break;
    case FIG_Z:
      block[1][0] = 1;
      block[1][1] = 1;
      block[2][1] = 1;
      block[2][2] = 1;
      break;
    case FIG_I:
      block[1][0] = 1;
      block[1][1] = 1;
      block[1][2] = 1;
      block[1][3] = 1;
      break;
    case FIG_T:
      block[1][0] = 1;
      block[1][1] = 1;
      block[1][2] = 1;
      block[1][3] = 1;
      break;
    case FIG_O:
      block[1][0] = 1;
      block[1][1] = 1;
      block[1][2] = 1;
      block[2][1] = 1;
      break;
  }
}

int** create_matrix(int rows, int cols) {
  int** matrix = calloc(rows, sizeof(int*));
  for (int i = 0; i < rows && matrix; ++i) {
    matrix[i] = calloc(cols, sizeof(int));
    if (matrix[i] == NULL) {
      remove_matrix(matrix, rows);
    }
  }
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
  if (game.pause && act != Terminate) return;

  if (act == Terminate) game.status = GAMEOVER;

  // if (act == Up) game.status = SHIFTING;

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
      block_attaching();
      break;
    case GAMEOVER:
      game_end();
      break;
    default:
      break;
  }
}

void block_attaching() {
  int count = check_row();
  update_score(count);
  update_level();
  check_overflow();
}

void check_overflow() {
  game.status = SPAWN;
  for (int i = 0; i < FIELD_COLS && game.status != GAMEOVER; ++i) {
    if (game.field[0][i]) {
      game.status = GAMEOVER;
      game.pause = -1;
    }
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

  if (game.score > game.high_score) {
    set_hight_score();
    game.high_score = game.score;
  }
}

void update_level() {
  game.level = game.score / LEVEL_STEP + 1;
  if (game.level >= LEVEL_MAX) {
    game.level = LEVEL_MAX;
    game.status = GAMEOVER;
  }
  game.speed = SPEED_START + SPEED_STEP * game.level;
}

int check_row() {
  int count = 0;
  for (int i = FIELD_ROWS - 1; i >= 0; --i) {
    int is_filled = TRUE;
    for (int j = 0; j < FIELD_COLS && is_filled; ++j) {
      if (game.field[i][j] == 0) is_filled = FALSE;
    }
    if (is_filled) {
      ++count;
      delete_row(i);
      ++i;
    }
  }
  return count;
}

void delete_row(int row) {
  for (int n = row; n > 0; --n) {
    for (int m = 0; m < FIELD_COLS; ++m) {
      game.field[n][m] = game.field[n - 1][m];
    }
  }
  for (int k = 0; k < FIELD_COLS; ++k) {
    game.field[0][k] = 0;
  }
}

void block_moving(UserAction_t act) {
  switch (act) {
    case Left:
      shift_left();
      break;
    case Right:
      shift_right();
      break;
    case Down:
    case Up:
      fall_down();
      break;
    case Action:
      rotate();  // matrix
      break;
    default:
      break;
  }
}

void rotate() {
  if (check_attached())
    game.status = ATTACHING;
  else
    game.status = ACTION;
}

void shift_left() {
  unpin_block();
  game.block_x--;
  if (check_attached()) {
    game.block_x++;
    pin_block();
  } else {
    pin_block();
  }
  game.status = ACTION;
}

void shift_right() {
  unpin_block();
  game.block_x++;
  if (check_attached()) {
    game.block_x--;
    pin_block();
  } else {
    pin_block();
  }
  game.status = ACTION;
}

void shift_down() {
  unpin_block();
  game.block_y++;
  if (check_attached()) {
    game.block_y--;
    pin_block();
    game.status = ATTACHING;
  } else {
    pin_block();
    game.status = ACTION;
  }
}

int check_attached() {
  int is_attached = FALSE;
  for (int i = 0; i < BLOCK_SIZE && is_attached == FALSE; ++i) {
    // if (game.block_y + BLOCK_SIZE > FIELD_ROWS) is_attached = TRUE;
    for (int j = 0; j < BLOCK_SIZE && is_attached == FALSE; ++j) {
      if (game.block_y + BLOCK_SIZE > FIELD_ROWS && game.block[i][j] != 0) is_attached = TRUE;

      if ((game.block_x + j < 0 || game.block_x + j > FIELD_COLS) &&
          game.field[game.block_y + i][game.block_x + j] != 0)
        is_attached = TRUE;
      // if (game.block_x + i < 0 || game.block_x + j > FIELD_COLS)
      //   is_attached = TRUE;
      // if (game.field[game.block_y + i][game.block_x + j] != 0)
      //   is_attached = TRUE;
      if (game.field[game.block_y + i][game.block_x + j] != 0)
        is_attached = TRUE;
    }
  }
  return is_attached;
}

void unpin_block() {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      if (game.block[i][j] != 0)
        game.field[game.block_y + i][game.block_x + j] = 0;
    }
  }
}

void pin_block() {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      if (game.block[i][j] != 0)
        game.field[game.block_y + i][game.block_x + j] = game.block[i][j];
    }
  }
  game.status = SPAWN;
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

// timeval t1, t2;
// double elapsedTime;

// //start timer
// gettimeofday(&t1, NULL);

// //do stuff...

// //stop timer
// gettimeofday(&t2, NULL);

// //compute
// elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
// elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;

// //convert to seconds
// delta = elapsedTime /1000;