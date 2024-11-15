#ifndef TETRIS_FRONT_H
#define TETRIS_FRONT_H

#include <string.h>

#include "defines.h"

UserAction_t get_signal();

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_overlay() {
  print_rectangle(0, BOARD_N + 1, 0, BOARD_M + 1);
  print_rectangle(0, BOARD_N + 1, BOARD_M + 2, BOARD_M + HUD_WIDTH + 3);

  print_rectangle(1, 3, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(4, 6, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(7, 13, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(14, 20, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);

  MVPRINTW(2, BOARD_M + 5, "LEVEL: ");
  MVPRINTW(5, BOARD_M + 5, "SCORE: ");
  MVPRINTW(8, BOARD_M + 5, "NEXT: ");
  MVPRINTW(15, BOARD_M + 5, "MANAGE:");
  MVPRINTW(16, BOARD_M + 5, "arrows - move");
  MVPRINTW(17, BOARD_M + 5, "space - rotate");
  MVPRINTW(18, BOARD_M + 5, "enter - pause");
  MVPRINTW(19, BOARD_M + 5, "esc - exit");
}

void print_banner(const char *banner) {
  MVPRINTW(BOARD_N / 2, (BOARD_M - BANNER_LEN) / 2 + 1, "%s", banner);
}

void print_stats(GameInfo_t gameInfo) {
  MVPRINTW(2, BOARD_M + 12, "%d", gameInfo.level);
  MVPRINTW(5, BOARD_M + 12, "%d", gameInfo.score);
}

void print_field(GameInfo_t gameInfo) {
  for (int i = 0; i < 20; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameInfo.field[i][j]) {
        MVPRINTW(i + 1, j * 2 + 1, "%c", '[');
        MVPRINTW(i + 1, j * 2 + 2, "%c", ']');
      } else {
        MVPRINTW(i + 1, j * 2 + 1, "%c", ' ');
        MVPRINTW(i + 1, j * 2 + 2, "%c", ' ');
      }
}

void print_screen(GameInfo_t gameInfo) {
  print_stats(gameInfo);
  if (gameInfo.pause == 1) {
    print_banner(INTRO_MESSAGE);
  } else if (gameInfo.pause == -1) {  //! game_over??
    print_banner(YOU_LOSE);
  } else if (gameInfo.level == 10) {
    print_banner(YOU_WON);
  } else
    print_field(gameInfo);
  refresh();
  napms(10);
}

UserAction_t get_signal() {
  int user_input = GET_USER_INPUT;
  UserAction_t act = Up;  // nothing happen
  if (user_input == KEY__ENTER)
    act = Pause;
  else if (user_input == KEY_ESCAPE)
    act = Terminate;
  else if (user_input == KEY_SPACE)
    act = Action;
  else if (user_input == KEY_LEFT)
    act = Left;
  else if (user_input == KEY_RIGHT)
    act = Right;
  else if (user_input == KEY_DOWN)
    act = Down;
  return act;
}


#endif