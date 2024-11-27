#include <check.h>

#include "test.h"

START_TEST(rotation_prepeare_left) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->block_x = 200;
  int res_left = 0;
  int res_right = 0;

  rotation_prepare(&res_right, &res_left);

  ck_assert(res_left == 1);
  ck_assert(res_right == 0);
  game_end();
}

START_TEST(rotation_prepeare_right) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->block_x = -2;
  int res_left = 0;
  int res_right = 0;

  rotation_prepare(&res_right, &res_left);

  ck_assert(res_left == 0);
  ck_assert(res_right == 1);
  game_end();
}

START_TEST(checking_attached) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->field[10][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] = 1;
  fall_down();

  int res = check_attached();

  ck_assert(res == 1);
  game_end();
}

START_TEST(time_checking) {
  game_init();
  Game_t *game = game_data();
  game->status = ACTION;
  game->start_time = 0;

  check_time();

  ck_assert(game->status == SHIFTING);
  game_end();
}

START_TEST(shifting_left) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  pin_block();

  shift_left();

  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2] == 7);
  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 2] == 0);
  ck_assert(game->block_x = (FIELD_COLS - BLOCK_SIZE) / 2 - 1);
  game_end();
}

START_TEST(shifting_left_attached) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  game->block_x = -1;
  pin_block();

  shift_left();

  ck_assert(game->field[1][0] == 7);
  game_end();
}

START_TEST(shifting_right) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  pin_block();

  shift_right();

  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 2] == 7);
  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 3] == 0);
  ck_assert(game->block_x = (FIELD_COLS - BLOCK_SIZE) / 2 + 1);
  game_end();
}

START_TEST(shifting_down) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  pin_block();

  shift_down();

  ck_assert(game->field[2][(FIELD_COLS - BLOCK_SIZE) / 2 + 2] == 7);
  ck_assert(game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 3] == 0);
  ck_assert(game->block_y = 1);
  game_end();
}

START_TEST(falling_down) {
  game_init();
  Game_t *game = game_data();
  game->block = create_matrix(BLOCK_SIZE, BLOCK_SIZE);
  fill_block(game->block, FIG_O);
  pin_block();

  fall_down();

  ck_assert(game->field[FIELD_ROWS - 1][(FIELD_COLS - BLOCK_SIZE) / 2 + 2] ==
            7);
  ck_assert(game->block_y = 16);
  ck_assert(game->status = ATTACHING);
  game_end();
}

Suite *block_moving_suit(void) {
  Suite *s;
  s = suite_create("Block moving tests");
  TCase *tc;
  tc = tcase_create("Block moving");
  tcase_add_test(tc, time_checking);
  tcase_add_test(tc, shifting_left);
  tcase_add_test(tc, shifting_right);
  tcase_add_test(tc, shifting_down);
  tcase_add_test(tc, falling_down);
  tcase_add_test(tc, checking_attached);
  tcase_add_test(tc, rotation_prepeare_left);
  tcase_add_test(tc, shifting_left_attached);
  tcase_add_test(tc, rotation_prepeare_right);

  suite_add_tcase(s, tc);
  return s;
}

int test_block_moving(void) {
  Suite *s = block_moving_suit();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}