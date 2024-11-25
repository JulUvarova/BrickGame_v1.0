#include "test.h"

int main(void) {
  int number_failed = 0;

  number_failed += sest();

  printf("====================\n\n");
  printf("Total errors: %d\n\n", number_failed);
  printf("====================\n\n");

  return 0;
}