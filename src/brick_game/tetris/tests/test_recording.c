#include <check.h>

#include "test.h"

START_TEST(update_level_) {
  game_init();
  Game_t* game = game_data();
  game->score = 600;

  update_level();

  ck_assert(game->level == 2);
  game_end();
}
END_TEST

START_TEST(update_max_level) {
  game_init();
  Game_t* game = game_data();
  game->score = 100000;

  update_level();

  ck_assert(game->level == LEVEL_MAX);
  game_end();
}
END_TEST

START_TEST(update_level_to_win) {
  game_init();
  Game_t* game = game_data();
  game->score = 60000;

  update_level();

  ck_assert(game->level == LEVEL_MAX);
  ck_assert(game->status == GAMEOVER);
  game_end();
}
END_TEST

START_TEST(get_or_set_hight_score) {
  game_init();
  Game_t* game = game_data();

  game->score = 100;
  set_hight_score();
  get_hight_score();

  ck_assert(game->high_score == 100);
  game_end();
}
END_TEST

START_TEST(update_score_one_line) {
  game_init();
  const Game_t* game = game_data();

  update_score(1);

  ck_assert(game->score == 100);
  game_end();
}
END_TEST

START_TEST(update_score_two_line) {
  game_init();
  const Game_t* game = game_data();

  update_score(2);

  ck_assert(game->score == 300);
  game_end();
}
END_TEST

START_TEST(update_score_three_line) {
  game_init();
  const Game_t* game = game_data();

  update_score(3);

  ck_assert(game->score == 700);
  game_end();
}
END_TEST

START_TEST(update_score_four_line) {
  game_init();
  const Game_t* game = game_data();

  update_score(4);

  ck_assert(game->score == 1500);
  game_end();
}
END_TEST

Suite* score_and_level_suite(void) {
  Suite* s;
  s = suite_create("Recording tests");
  TCase* tc;
  tc = tcase_create("Score & level");
  tcase_add_test(tc, get_or_set_hight_score);
  tcase_add_test(tc, update_score_one_line);
  tcase_add_test(tc, update_score_two_line);
  tcase_add_test(tc, update_score_three_line);
  tcase_add_test(tc, update_score_four_line);
  tcase_add_test(tc, update_level_);
  tcase_add_test(tc, update_level_to_win);
  tcase_add_test(tc, update_max_level);

  suite_add_tcase(s, tc);
  return s;
}

int test_recording(void) {
  Suite* s = score_and_level_suite();
  SRunner* sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}