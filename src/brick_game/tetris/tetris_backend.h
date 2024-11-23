#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdlib.h>
#include <time.h>

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
void check_time();
unsigned long long get_time();

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
int** copy_matrix(int** src, int size);

void rotate_matrix();
void shift_left();
void shift_right();
void fall_down();
void shift_down();
void rotate();
int check_attached();
void rotation_prepare(int* is_shift_right, int* is_shift_left);

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

  // game.start_time = get_time();
  game.left_time = 0;
}

void game_end() {
  remove_matrix(game.field, FIELD_ROWS);
  remove_matrix(game.next, BLOCK_SIZE);
  remove_matrix(game.block, BLOCK_SIZE);
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

  game.rotate = (game.block_name == FIG_S || game.block_name == FIG_Z) ? 3 : 1;

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
      FILL_L(block);
      break;
    case FIG_J:
      FILL_J(block);
      break;
    case FIG_S:
      FILL_S(block);
      break;
    case FIG_Z:
      FILL_Z(block);
      break;
    case FIG_I:
      FILL_I(block);
      break;
    case FIG_T:
      FILL_T(block);
      break;
    case FIG_O:
      FILL_O(block);
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

int** copy_matrix(int** src, int size) {
  int** dest = create_matrix(size, size);
  if (dest) {
    for (int i = 0; i < BLOCK_SIZE; ++i) {
      for (int j = 0; j < BLOCK_SIZE; ++j) {
        dest[i][j] = src[i][j];
      }
    }
  }
  return dest;
}

void rotate_matrix() {
  int** new = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  if (new) {
    int size = BLOCK_SIZE - 1;
    if (game.block_name == FIG_I) ++size;

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        new[j][size - 1 - i] = game.block[i][j];
      }
    }
    for (int i = 0; i < BLOCK_SIZE; ++i) {
      for (int j = 0; j < BLOCK_SIZE; ++j) {
        game.block[i][j] = new[i][j];
      }
    }
    remove_matrix(new, BLOCK_SIZE);
  }
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
  if (!act) game.status = SHIFTING;
  // check_time();

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

void check_time() {
  if ((get_time() - game.start_time) > SPEED_START) {
    game.status = SHIFTING;
    game.start_time = get_time();
  }
}

unsigned long long get_time() {
  unsigned long result = 0;
  struct timespec now = {0};
  if (timespec_get(&now, TIME_UTC) != 0)
    result = (unsigned long long)now.tv_sec * 1000 + now.tv_nsec / 1000000;
  return result;
}

void block_attaching() {
  game.status = SPAWN;
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
  int old_level = game.level;
  game.level = game.score / LEVEL_STEP + 1;
  if (game.level >= LEVEL_MAX) {
    game.level = LEVEL_MAX;
    game.status = GAMEOVER;
  }
  if (game.level > old_level) game.speed -= SPEED_STEP;
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
      shift_down();
      break;
    case Up:
      rotate();
      break;
    case Action:
      fall_down();
      break;
    default:
      break;
  }
}

void rotate() {
  if (game.block_name == FIG_O) return;

  unpin_block();
  int** tmp = game.block;
  game.block = copy_matrix(tmp, BLOCK_SIZE);

  for (int i = 0; i < game.rotate; ++i) rotate_matrix();

  int is_shift_right = FALSE;
  int is_shift_left = FALSE;
  rotation_prepare(&is_shift_right, &is_shift_left);
  if (is_shift_right) ++game.block_x;
  if (is_shift_left) --game.block_x;

  if (check_attached()) {
    if (is_shift_right) --game.block_x;
    if (is_shift_left) ++game.block_x;
    remove_matrix(game.block, BLOCK_SIZE);
    game.block = tmp;
  } else if (game.block_name == FIG_I || game.block_name == FIG_S ||
             game.block_name == FIG_Z) {
    game.rotate = (game.rotate == 1) ? 3 : 1;
    remove_matrix(tmp, BLOCK_SIZE);
  }
  pin_block();

  game.status = ACTION;
}
void rotation_prepare(int* is_shift_right, int* is_shift_left) {
  if (game.block_x < 0) {
    for (int i = 0; i < BLOCK_SIZE; ++i) {
      if (game.block[i][0] != 0) *is_shift_right = TRUE;
    }
  }
  if (game.block_x > FIELD_COLS - 1) {
    for (int i = 0; i < BLOCK_SIZE; ++i) {
      if (game.block[i][0] != 0) *is_shift_left = TRUE;
    }
  }
}

void shift_left() {
  unpin_block();
  game.block_x--;
  if (check_attached()) {
    game.block_x++;
  }
  pin_block();
  game.status = ACTION;
}

void shift_right() {
  unpin_block();
  game.block_x++;
  if (check_attached()) {
    game.block_x--;
  }
  pin_block();
  game.status = ACTION;
}

void shift_down() {
  unpin_block();
  game.block_y++;
  if (check_attached()) {
    game.block_y--;
    game.status = ATTACHING;
  } else {
    game.status = ACTION;
  }
  pin_block();
}

int check_attached() {
  int is_attached = FALSE;
  for (int i = 0; i < BLOCK_SIZE && is_attached == FALSE; ++i) {
    if (game.block_y + i < 0) continue;
    for (int j = 0; j < BLOCK_SIZE && is_attached == FALSE; ++j) {
      if (game.block[i][j] != 0 &&
          (game.block_x + j < 0 || game.block_x + j > FIELD_COLS - 1 ||
           game.block_y + i > FIELD_ROWS - 1 ||
           game.field[i + game.block_y][j + game.block_x]))
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
    fclose(file);
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