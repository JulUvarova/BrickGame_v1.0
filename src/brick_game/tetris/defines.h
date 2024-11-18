#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    halfdelay(10);       \
    keypad(stdscr, TRUE); \
    timeout(10);        \
  }

#define GET_USER_INPUT getch()

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

#define YOU_WON "   =) YOU WIN! =)   "
#define YOU_LOSE "   =( YOU LOSE =(   "
#define INTRO_MESSAGE "Press ENTER to start"
#define BANNER_LEN 20

#define SPEED_START 5
#define SPEED_STEP 5

#define LEVEL_MAX 10
#define LEVEL_STEP 600
#define COST_ONE_LINE 100
#define COST_TWO_LINE 300
#define COST_THREE_LINE 700
#define COST_FOUR_LINE 1500

#define BOARDS_BEGIN 2

#define BOARD_N 20
#define BOARD_M 20
#define HUD_WIDTH 17
#define MAP_PADDING 3

#define KEY_ESCAPE 27
#define KEY_SPACE ' '
#define KEY__ENTER '\n'

#define HIGH_SCORE_MEM "record.txt"

#endif