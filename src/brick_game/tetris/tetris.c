#include <locale.h>
#include <ncurses.h>

#include "objects.h"
#include "tetris_backend.h"
#include "tetris_frontend.h"

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  print_overlay();
  // bool stop_flag = TRUE;
  GameInfo_t gameInfo;
  game_init(&gameInfo);
  // int get = GET_USER_INPUT;
  // if (get == '\r') {
  print_screen(&gameInfo);

  keypad(stdscr, TRUE);
  int a = GET_USER_INPUT;
  if (a == KEY_LEFT) {
    gameInfo.pause = FALSE;
    print_screen(&gameInfo);
    napms(5000);
  }
  // }
  // while (stop_flag) {

  // }

  // bool stop_flag = true;
  // while (stop_flag) {

  // }

  //   game_loop();
  endwin();
  return 0;
}
