#ifndef OBJECTS_H
#define OBJECTS_H

#include <sys/time.h>

#define SPEED_START 5000
#define SPEED_STEP 200

#define LEVEL_MAX 10
#define LEVEL_STEP 600

#define COST_ONE_LINE 100
#define COST_TWO_LINE 300
#define COST_THREE_LINE 700
#define COST_FOUR_LINE 1500

#define FILL_L(block) \
  {                   \
    block[0][1] = 1;  \
    block[1][1] = 1;  \
    block[2][1] = 1;  \
    block[2][2] = 1;  \
  }
#define FILL_J(block) \
  {                   \
    block[0][1] = 2;  \
    block[1][1] = 2;  \
    block[2][1] = 2;  \
    block[2][0] = 2;  \
  }

#define FILL_S(block) \
  {                   \
    block[1][1] = 3;  \
    block[1][2] = 3;  \
    block[2][1] = 3;  \
    block[2][0] = 3;  \
  }

#define FILL_Z(block) \
  {                   \
    block[1][0] = 4;  \
    block[1][1] = 4;  \
    block[2][1] = 4;  \
    block[2][2] = 4;  \
  }
#define FILL_I(block) \
  {                   \
    block[1][0] = 5;  \
    block[1][1] = 5;  \
    block[1][2] = 5;  \
    block[1][3] = 5;  \
  }
#define FILL_T(block) \
  {                   \
    block[1][0] = 6;  \
    block[1][1] = 6;  \
    block[1][2] = 6;  \
    block[0][1] = 6;  \
  }
#define FILL_O(block) \
  {                   \
    block[1][1] = 7;  \
    block[1][2] = 7;  \
    block[2][1] = 7;  \
    block[2][2] = 7;  \
  }
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

  int next_name;
  int **next;
  int block_name;
  int **block;
  int block_x;
  int block_y;
  int rotate;

  int score;
  int high_score;
  int level;
  int speed;

  unsigned long long start_time;
  unsigned long long left_time;
} GameInfo_t;

typedef enum {
  FIG_J = 0,
  FIG_L,
  FIG_O,
  FIG_I,
  FIG_Z,
  FIG_S,
  FIG_T
} Block_name_t;

static GameInfo_t game = {START, 1, NULL, 0, NULL, 0, NULL, 0,
                          0,     0, 0,    0, 1,    0, 0,    0};

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