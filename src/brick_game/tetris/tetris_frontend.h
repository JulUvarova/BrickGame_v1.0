#ifndef TETRIS_FRONT_H
#define TETRIS_FRONT_H

#include <string.h>

#include "defines.h"

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

void print_banner() {
  MVPRINTW(BOARD_N / 2, (BOARD_M - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);
}

void print_stats(GameInfo_t *gameInfo) {
  MVPRINTW(2, BOARD_M + 12, "%d", gameInfo->level);
  MVPRINTW(5, BOARD_M + 12, "%d", gameInfo->score);
}

void print_field(GameInfo_t *gameInfo) {
   for (int i = 0; i < 20; ++i)
     for (int j = 0; j < 10; ++j)
       if (!gameInfo->field[i][j]) {
         MVPRINTW(i + 1, j * 2 + 1, "%c", '[');
         MVPRINTW(i + 1, j * 2 + 2, "%c", ']');
       }
}

void print_screen(GameInfo_t *gameInfo) {
  print_stats(gameInfo);
  if (gameInfo->pause)
    print_banner();
  else
    print_field(gameInfo);
  refresh();
  napms(2000);
}

// void print_board(board_t *game, player_pos *frog)
// {
//     print_cars(game);
//     PRINT_FROG(frog->x, frog->y);
// }

// void print_cars(board_t *game)
// {
//     for(int i = MAP_PADDING + 1; i < BOARD_N - MAP_PADDING + 1; i++)
//     {
//         if (i % 2 == (MAP_PADDING + 1) % 2)
//         {
//             for (int j = 1; j < BOARD_M + 1; j++)
//                 MVADDCH(i, j, ACS_BLOCK);
//         }
//         else
//         {
//             for (int j = 1; j < BOARD_M + 1; j++)
//             {
//                 if (game->ways[i - MAP_PADDING - 1][j - 1] == '0')
//                     MVADDCH(i, j, ' ');
//                 else
//                     MVADDCH(i, j, ']');
//             }
//         }
//     }
// }

// void print_finished(board_t *game)
// {
//     for (int i = 0; i < BOARD_M; i++)
//     {
//         if (game->finish[i] == '0')
//             MVADDCH(1, i + 1, ACS_BLOCK);
//         else
//             MVADDCH(1, i + 1, ' ');
//     }
// }
// void print_banner(game_stats_t *stats)
// {
//     banner_t banner;

//     memset(banner.matrix, 0, (BANNER_N + 1) * (BANNER_M + 1));

//     clear();

//     if (!(read_banner(stats, &banner)))
//     {
//         for (int i = 0; i < BANNER_N; i++)
//             for (int j = 0; j < BANNER_M; j++)
//                 if (banner.matrix[i][j] == '#')
//                     MVADDCH(i, j, ACS_BLOCK);
//                 else
//                     MVADDCH(i, j, ' ');
//         refresh();
//         napms(2000);
//     }
// }

// int read_banner(game_stats_t *stats, banner_t *banner)
// {
//     int rc = SUCCESS;
//     FILE *file = NULL;

//     if (stats->lives)
//         file = fopen(YOU_WON, "r");
//     else
//         file = fopen(YOU_LOSE, "r");

//     if (file)
//     {
//         for (int i = 0; i < BANNER_N - 1 && !rc; i++)
//         {
//             if (fgets(banner->matrix[i], BANNER_M + 2, file) == NULL)
//                 rc = ERROR;
//             else
//                 banner->matrix[i][strcspn(banner->matrix[i], "\n")] = '\0';
//         }

//         fclose(file);
//     }
//     else
//         rc = ERROR;

//     return rc;
// }

#endif