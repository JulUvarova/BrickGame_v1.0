#ifndef OBJECTS_H
#define OBJECTS_H

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum {
  START,
  SPAWN,
  ACTION,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  ERROR_STATE
} State_t;

typedef struct {
  State_t status;
  int pause;

  int **field;

  int **next;
  int **block;
  int block_size;
  int block_x;
  int block_y;

  int score;
  int high_score;
  int level;
  int speed;

  unsigned long long start_time;
  unsigned long long left_time;
} GameInfo_t;

static GameInfo_t game = {START, 1, NULL, NULL, NULL, 0, 0,
                          0,     0, 0,    1,    0,    0, 0};

// typedef struct {
//   int **field;
//   int **next;
//   int score;
//   int high_score;
//   int level;
//   int speed;
//   int pause;
// } GameInfo_t;

#endif