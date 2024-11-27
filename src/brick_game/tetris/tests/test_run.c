#include "test.h"

int main(void) {
  int number_failed = 0;

  number_failed += test_recording();
  number_failed += test_status();
  number_failed += test_matrix();
  number_failed += test_block_moving();
  number_failed += test_spec_func();

  printf("====================\n\n");
  printf("Total errors: %d\n\n", number_failed);
  printf("====================\n\n");

  return 0;
}