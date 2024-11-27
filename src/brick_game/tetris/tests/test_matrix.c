#include <check.h>

#include "test.h"

START_TEST(check_and_delete_rows) {
  game_init();
  Game_t *game = game_data();

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < FIELD_COLS; ++j) game->field[i][j] = 1;
  }

  int res = check_row();
  
  ck_assert(res == 4);
  game_end();
}

START_TEST(fill_block_J) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_J);

  ck_assert(game->block[0][1] == 2);
  ck_assert(game->block[2][0] == 2);
  game_end();
}

START_TEST(fill_block_L) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_L);

  ck_assert(game->block[0][1] == 1);
  ck_assert(game->block[2][2] == 1);
  game_end();
}

START_TEST(fill_block_S) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_S);

  ck_assert(game->block[1][1] == 3);
  ck_assert(game->block[2][0] == 3);
  game_end();
}

START_TEST(fill_block_Z) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_Z);

  ck_assert(game->block[1][0] == 4);
  ck_assert(game->block[2][2] == 4);
  game_end();
}

START_TEST(fill_block_I) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_I);

  ck_assert(game->block[1][0] == 5);
  ck_assert(game->block[1][3] == 5);
  game_end();
}

START_TEST(fill_block_T) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_T);

  ck_assert(game->block[1][0] == 6);
  ck_assert(game->block[0][1] == 6);
  game_end();
}

START_TEST(pin_block_) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);

  pin_block();

  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] == 7);
  game_end();
}

START_TEST(unpin_block_) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  pin_block();

  unpin_block();

  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] == 0);
  game_end();
}

START_TEST(rotate_matrix_block) {
  game_init();
  Game_t *game = game_data();
  block_spawn();
  game->block[0][0] = 1;
  game->block[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = 1;

  rotate_matrix();

  ck_assert(game->block[0][2] = 1);
  ck_assert(game->block[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = 1);
  game_end();
}

START_TEST(create_matrix_block) {
  int **matrix = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  matrix[0][0] = 1;
  matrix[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = 1;

  ck_assert(matrix[0][0] = 1);
  ck_assert(matrix[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = 1);
  remove_matrix(matrix, BLOCK_SIZE);
}

START_TEST(copy_matrix_block) {
  int **matrix = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  matrix[0][0] = 1;
  matrix[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = 1;

  int **copied = copy_matrix(matrix, BLOCK_SIZE);

  ck_assert(matrix[0][0] = copied[0][0]);
  ck_assert(matrix[BLOCK_SIZE - 1][BLOCK_SIZE - 1] =
                copied[BLOCK_SIZE - 1][BLOCK_SIZE - 1]);
  remove_matrix(matrix, BLOCK_SIZE);
  remove_matrix(copied, BLOCK_SIZE);
}

Suite *matrix_works_suite(void) {
  Suite *s;
  s = suite_create("Matrix tests");
  TCase *tc;
  tc = tcase_create("Matrix works");
  tcase_add_test(tc, create_matrix_block);
  tcase_add_test(tc, copy_matrix_block);
  tcase_add_test(tc, rotate_matrix_block);
  tcase_add_test(tc, pin_block_);
  tcase_add_test(tc, unpin_block_);
  tcase_add_test(tc, fill_block_I);
  tcase_add_test(tc, fill_block_L);
  tcase_add_test(tc, fill_block_J);
  tcase_add_test(tc, fill_block_Z);
  tcase_add_test(tc, fill_block_S);
  tcase_add_test(tc, fill_block_T);
  tcase_add_test(tc, check_and_delete_rows);

  suite_add_tcase(s, tc);
  return s;
}

int test_matrix(void) {
  Suite *s = matrix_works_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}