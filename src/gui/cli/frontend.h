#ifndef FRONT_H
#define FRONT_H

#include <ncurses.h>
#include <string.h>

#include "../../common_objects/objects.h"

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(10);          \
  }

#define COLOR_INIT()                            \
  {                                             \
    start_color();                              \
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);   \
    init_pair(2, COLOR_MAGENTA, COLOR_MAGENTA); \
    init_pair(3, COLOR_GREEN, COLOR_GREEN);     \
    init_pair(4, COLOR_CYAN, COLOR_CYAN);       \
    init_pair(5, COLOR_RED, COLOR_RED);         \
    init_pair(6, COLOR_BLUE, COLOR_BLUE);       \
    init_pair(7, COLOR_WHITE, COLOR_WHITE);     \
    init_pair(8, COLOR_CYAN, COLOR_BLACK);      \
  }

#define GET_USER_INPUT getch()

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

#define YOU_WON "   =) YOU WIN! =)   "
#define YOU_LOSE "   =( YOU LOSE =(   "
#define INTRO_MESSAGE "Press ENTER to start"
#define BANNER_LEN 20

#define BOARDS_BEGIN 2

#define BOARD_N 20
#define BOARD_M 20
#define HUD_WIDTH 17
#define MAP_PADDING 3

#define KEY_ESCAPE 27
#define KEY_SPACE ' '
#define KEY__ENTER '\n'

UserAction_t get_signal();

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

void print_overlay();

void print_banner(const char* banner);

void print_stats(GameInfo_t gameInfo);

void print_next(int** next);

void print_field(int** field);

void print_screen(GameInfo_t gameInfo);

#endif