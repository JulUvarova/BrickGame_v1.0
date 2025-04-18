#include <locale.h>

#include "brick_game/tetris/tetris_backend.h"
#include "gui/cli/frontend.h"

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
    GameInfo_t game_info = updateCurrentState();
    print_screen(game_info);
    userInput(get_signal(), 0);
    game_info = updateCurrentState();
    if (game_info.pause == -2) stop_flag = FALSE;
  }
}
