#include <locale.h>
#include <ncurses.h>

#include "objects.h"
#include "tetris_backend.h"
#include "tetris_frontend.h"

int check_end(GameInfo_t* game_info);
void game_loop();

int main(void) {
  WIN_INIT(50);
  if (has_colors()) {
    COLOR_INIT();
  }
  setlocale(LC_ALL, "");
  print_overlay();

  game_loop();

  endwin();
  return 0;
}

void game_loop() {
  bool stop_flag = TRUE;
  while (stop_flag) {
    print_screen(updateCurrentState());
    userInput(get_signal(), false);
    if (game.status == EXIT_STATE) stop_flag = FALSE;
  }
}

int check_end(GameInfo_t* game_info) {
  int exit = 0;
  if (game_info->field == NULL || game_info->next == NULL ||
      game_info->level == 10)
    exit = 1;
  return exit;
}
