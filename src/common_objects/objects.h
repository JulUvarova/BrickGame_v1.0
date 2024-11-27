/*!
\file
\brief Заголовочный файл со структурами обмена между фронтендом и бекендом
\author Trumpecy
\date Ноябрь 2024 года

Данный файл содержит в себе определения основных
структук и функций, используемых для обмена между фронтендом и бекендом
*/
#ifndef OBJECTS_H
#define OBJECTS_H

/// Список возможных клавиш управления
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

/*!
  \brief Структура для хранения информации об игре, необходимой для прорисовки экрана
*/
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif