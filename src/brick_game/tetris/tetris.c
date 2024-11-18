#include <locale.h>
#include <ncurses.h>

#include "objects.h"
#include "tetris_backend.h"
#include "tetris_frontend.h"

int check_end(GameInfo_t* game_info);

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  print_overlay();

  // -- game_loop();
  bool stop_flag = TRUE;
  while (stop_flag) {
    userInput(get_signal(), false);
    print_screen(updateCurrentState());

    if (game.status == EXIT_STATE) stop_flag = FALSE;
    // flushinp();
  }
  if (game.level > 9 || game.pause < 0) napms(5000);

  // -- end game_loop();
  endwin();
  return 0;
}

int check_end(GameInfo_t* game_info) {
  int exit = 0;
  if (game_info->field == NULL || game_info->next == NULL ||
      game_info->level == 10)
    exit = 1;
  return exit;
}
