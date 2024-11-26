#include <check.h>

#include "test.h"

void game_init();
void game_end();
void block_spawn();
void block_moving(UserAction_t act);
void block_attaching();
void check_overflow();

START_TEST(block_moving_all) {
  game_init();
  block_spawn();
  const Game_t *game = game_data();

  block_moving(Left);
  block_moving(Right);
  block_moving(Down);
  block_moving(Up);
  block_moving(Action);

  ck_assert(game->status == ATTACHING);
  game_end();
}

START_TEST(check_overflow_gameover) {
  game_init();
  Game_t *game = game_data();
  game->field[0][0] = 1;

  check_overflow();

  ck_assert(game->status == GAMEOVER);
  game_end();
}

START_TEST(check_overflow_spawn) {
  game_init();
  const Game_t *game = game_data();

  check_overflow();

  ck_assert(game->status == SPAWN);
  game_end();
}

START_TEST(block_attaching_spawn) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->field[10][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] = 1;
  fall_down();

  block_attaching();

  ck_assert(game->status == SPAWN);
  game_end();
}

START_TEST(block_attaching_gameover) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] = 1;

  block_attaching();

  ck_assert(game->field != NULL);
  ck_assert(game->next != NULL);
  ck_assert(game->block != NULL);
  ck_assert(game->block_x == (FIELD_COLS - BLOCK_SIZE) / 2);
  ck_assert(game->rotate != 0);
  ck_assert(game->status == GAMEOVER);
  game_end();
}

START_TEST(block_spawn_action) {
  game_init();
  const Game_t *game = game_data();

  block_spawn();

  ck_assert(game->field != NULL);
  ck_assert(game->next != NULL);
  ck_assert(game->block != NULL);
  ck_assert(game->block_x == (FIELD_COLS - BLOCK_SIZE) / 2);
  ck_assert(game->rotate != 0);
  ck_assert(game->status == ACTION);
  game_end();
}

START_TEST(block_spawn_gameover) {
  game_init();
  Game_t *game = game_data();
  game->field[1][(FIELD_COLS - BLOCK_SIZE) / 2 + 1] = 1;

  block_spawn();

  ck_assert(game->field != NULL);
  ck_assert(game->next != NULL);
  ck_assert(game->block != NULL);
  ck_assert(game->block_x == (FIELD_COLS - BLOCK_SIZE) / 2);
  ck_assert(game->rotate != 0);
  ck_assert(game->status == GAMEOVER);
  game_end();
}

START_TEST(game_init_spawn) {
  game_init();
  const Game_t *game = game_data();

  ck_assert(game->status == SPAWN);
  game_end();
}

START_TEST(game_end_exit) {
  game_init();
  block_spawn();
  const Game_t *game = game_data();

  game_end();

  ck_assert(game->field == NULL);
  ck_assert(game->next == NULL);
  ck_assert(game->block == NULL);
  ck_assert(game->pause == -2);
  ck_assert(game->status == EXIT_STATE);
}

Suite *fms_status_suite(void) {
  Suite *s;
  s = suite_create("Status tests");
  TCase *tc;
  tc = tcase_create("FSM works");
  tcase_add_test(tc, game_init_spawn);
  tcase_add_test(tc, game_end_exit);
  tcase_add_test(tc, block_spawn_action);
  tcase_add_test(tc, block_spawn_gameover);
  tcase_add_test(tc, block_attaching_gameover);
  tcase_add_test(tc, block_attaching_spawn);
  tcase_add_test(tc, check_overflow_gameover);
  tcase_add_test(tc, check_overflow_spawn);
  tcase_add_test(tc, block_moving_all);

  suite_add_tcase(s, tc);
  return s;
}

int test_status(void) {
  Suite *s = fms_status_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}