#include <check.h>

#include "test.h"

START_TEST(s21_sub_matrix_NULL_res) {
  matrix_t matrix = {0}, res = {0};
  int row = 2, col = 3;
  s21_create_matrix(row, col, &matrix);
  int res_code = s21_sub_matrix(&matrix, NULL, &res);
  s21_remove_matrix(&matrix);

  ck_assert(res_code == INCORRECT_MATRIX);
}
END_TEST

Suite *s21_sub_matrix_suite(void) {
  Suite *s;
  s = suite_create("s21_sub_matrix() tests");
  TCase *tc;
  tc = tcase_create("sub_matrix");
  tcase_add_test(tc, s21_sub_matrix_NULL_res);

  suite_add_tcase(s, tc);
  return s;
}

int s21_sub_matrix_test(void) {
  Suite *s = s21_sub_matrix_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}