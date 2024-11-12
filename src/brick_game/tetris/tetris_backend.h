#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdlib.h>

#include "defines.h"
#include "objects.h"

void game_init(GameInfo_t *gameInfo);
int **create_field();
void remove_field(int **field);

void game_init(GameInfo_t *gameInfo) {
  gameInfo->field = create_field();
  gameInfo->next = NULL;
  gameInfo->score = 0;
  gameInfo->high_score = 0;  //! func
  gameInfo->level = 1;
  gameInfo->speed = 1;
  gameInfo->pause = TRUE;
}

int **create_field() {
  int **field = calloc(BOARD_N, sizeof(int *));
  for (int i = 0; i < BOARD_N && field != NULL; ++i) {
    field[i] = calloc(BOARD_M / 2, sizeof(int));
    if (field[i] == NULL) {
      remove_field(field);
    }
  }
  return field;
}

void remove_field(int **field) {
  if (field != NULL) {
    for (int i = 0; i < BOARD_N && field[i] != NULL; ++i) {
      free(field[i]);
    }
    free(field);
    field = NULL;
  }
}

UserAction_t get_signal(int user_input) {
  UserAction_t act = Up;
  if (user_input == KEY_ENTER)
    act = Pause;
  else if (user_input == ESC)
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