#include <check.h>

#include "test.h"

START_TEST(update_current_state) {
  game_init();
  block_spawn();
  const Game_t *game = game_data();

  GameInfo_t game_info = updateCurrentState();

  ck_assert(game->field == game_info.field);
  ck_assert(game->next == game_info.next);
  ck_assert(game->score == game_info.score);
  ck_assert(game->high_score == game_info.high_score);
  ck_assert(game->level == game_info.level);
  ck_assert(game->speed == game_info.speed);
  ck_assert(game->pause == game_info.pause);
  game_end();
}

START_TEST(input_pause) {
  userInput(Pause, 0);
  const Game_t *game = game_data();

  ck_assert(game->status == ACTION);
  game_end();
}

START_TEST(input_terminate) {
  userInput(Terminate, 0);
  const Game_t *game = game_data();

  ck_assert(game->status == GAMEOVER);
  game_end();
}

START_TEST(input_moving) {
  game_init();
  block_spawn();
  userInput(Left, 0);
  userInput(Right, 0);
  userInput(Up, 0);
  userInput(Down, 0);
  userInput(Action, 0);
  const Game_t *game = game_data();

  ck_assert(game->status == ATTACHING);
  game_end();
}

START_TEST(input_in_attaching) {
  game_init();
  block_spawn();
  userInput(Action, 0);
  const Game_t *game = game_data();
  userInput(1111111, 0);

  ck_assert(game->status == SPAWN);
  game_end();
}

START_TEST(input_in_shifting) {
  game_init();
  block_spawn();
  Game_t *game = game_data();
  game->start_time = -1;
  userInput(Action, 0);

  ck_assert(game->status == ACTION);
  game_end();
}

START_TEST(input_for_start) {
  game_init();
  Game_t *game = game_data();
  remove_matrix(game->field, FIELD_ROWS);
  game->status = START;
  userInput(Pause, 0);

  ck_assert(game->status == SPAWN);
  game_end();
}

START_TEST(input_in_spawn) {
  game_init();
  Game_t *game = game_data();
  game->status = SPAWN;
  userInput(Left, 0);

printf("\n%d\n", game->status);
  ck_assert(game->status == ACTION);
  game_end();
}

Suite *spec_func_suite(void) {
  Suite *s;
  s = suite_create("Specification functions tests");
  TCase *tc;
  tc = tcase_create("API works");
  tcase_add_test(tc, update_current_state);
  tcase_add_test(tc, input_pause);
  tcase_add_test(tc, input_terminate);
  tcase_add_test(tc, input_moving);
  tcase_add_test(tc, input_in_attaching);
  tcase_add_test(tc, input_in_shifting);
  tcase_add_test(tc, input_for_start);
  tcase_add_test(tc, input_in_spawn);

  suite_add_tcase(s, tc);
  return s;
}

int test_spec_func(void) {
  Suite *s = spec_func_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}