/*!
\file
\brief Заголовочный файл с описанием библиотеки логики тетриса
\author Trumpecy
\date Ноябрь 2024 года

Данный файл содержит в себе определения основных
структук и функций, используемых для сборки бекэнда игры
*/
#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../common_objects/objects.h"

#define TRUE 1
#define FALSE 0

#define FIELD_COLS 10
#define FIELD_ROWS 20

#define BLOCK_SIZE 4

#define HIGH_SCORE_MEM "highest_score.txt"

#define SPEED_START 3000

#define LEVEL_MAX 2
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

/// Список возможных состояний конечного автомата
typedef enum {
  START = 0,
  SPAWN,
  ACTION,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE
} State_t;

/*!
  \brief Структура для хранения полной информации об игре. Представлена внутри
  библиотеки игры статическим объектом.
*/
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
} Game_t;

/// Список возможных фигур
typedef enum {
  FIG_J = 0, ///< Указывает, что элемент недоступен для использования
  FIG_L,
  FIG_O,
  FIG_I,
  FIG_Z,
  FIG_S,
  FIG_T
} Block_name_t;

/*!
    \defgroup main Функции для работы с библиотекой
    \brief Содержит основые функции для ввода данных и вывода информации об игре
*/

/*!
\ingroup main
Вход в библиотеку бекенда игры.
Принимает информацию о действии пользователя
\param[in] action Действие пользователя (клавиша, простой)
\param[in] hold Не используется в данной игре
*/
void userInput(UserAction_t action, int hold);

/*!
\ingroup main
Выход из библиотеки бекенда игры.
Копирует требуемые поля статического объекта данных игры для передачи в
интерфейс на отрисовку
*/
GameInfo_t updateCurrentState();

/*!
    \defgroup score Функции для работы с подсчетом очков и уровней
    \brief Позволяет подсчитывать очки, сохранять рекорд, повышать уровень и
   изменять скорость игры
*/

/*!
\ingroup score
Считывает данные о ранее установленном рекорде из файла highest_score.txt,
сохраняет их в поле hight_score
*/
void get_hight_score();

/*!
\ingroup score
Заносит данные о новом установленном рекорде в файл highest_score.txt из поля
hight_score
*/
void set_hight_score();

/*!
\ingroup score
Увеличивает поле score на величину, зависящую от count заполненных рядов
\param[in] count количество заполенных рядов 
*/
void update_score(int count);

/*!
\ingroup score
Увеличивает поле level в зависимости от величины score.
При достижении LEVEL_MAX игра переходит в состояние GAMEOVER
*/
void update_level();

/*!
    \defgroup status Функции для работы с состояниями игры
    \brief Включают инициализацию игры, смену состояний и окончание
*/

/*!
\ingroup status
Инициализирует структуру Game_t, создает игровое поле, устанавливает начальные значения игры
Срабатывает при статусе START
Устанавливает состояние SPAWN
Вслучае невыделения памяти под field статус меняется на GAMEOVER
*/
void game_init();

/*!
\ingroup status
Уничтожает матрицы игрового поля, текущего и следующего блоков
Устанавливает значение поля pause -2 - сигнал завешения работы бекенда
Срабатывает при статусе GAMEOVER
Устанавливает состояние EXIT_GAME
*/
void game_end();

/*!
\ingroup status
Создает матрицы текущего и следующего блоков, заполняет их рандомной фигурой, устанавливает параметры блоков
Срабатывает при статусе SPAWN
При заполнении первой строки игрового поля переводит состояние на GAMEOVER, иначе - ACTION
*/
void block_spawn();

/*!
\ingroup status
Вызыает функции обработки движения в зависимости от выбранного пользователем действия
\param[in] action Действие пользователя: вправо, влево, вниз, падение, ротация, простой
Срабатывает при статусе ACTION
*/
void block_moving(UserAction_t act);

/*!
\ingroup status
Проверка столкновения фигруры с нижней рамкой или блоками на поле
Срабатывает при статусе ATTACHING
Переводит статус на SPAWN
*/
void block_attaching();

/*!
\ingroup status
Проверка касания точек столкнувшейся фигуры с верхней строкой игрового поля
Переводит статус на SPAWN, если столкновения не было, иначе - GAMEOVER
*/
void check_overflow();

/*!
    \defgroup move Функции, обеспечивающие движение блоков на игровом поле
    \brief Включают в себя основные действия: движения влево, вправо, вниз,
   поворот, падение
*/

/*!
\ingroup move
Считает разницу между start_time и текущем временем, сравнивает ее сщ значением speed
Если разница больше - переход в статус SHIFTING
Срабатывает при состоянии ACTION
*/
void check_time();

/*!
\ingroup move
Считает и возвращает время в милисекундах с помощью struct timespec
*/
unsigned long long get_time();

/*!
\ingroup move
Сдвигает фигуру влево, если нет мешающих элементов
*/
void shift_left();

/*!
\ingroup move
Сдвигает фигуру вправо, если нет мешающих элементов
*/
void shift_right();

/*!
\ingroup move
Повторяет команду shift_down(), пока статус не сменится на ATTACHING
*/
void fall_down();

/*!
\ingroup move
Сдвигает фигуру на клетку ниже
При столкновении с другими фигурами статус меняется на ATTACHING, иначе - ACTION
*/
void shift_down();

/*!
\ingroup move
Вращает фигуру rotate раз, если нет мешающих элементов
*/
void rotate();

/*!
\ingroup move
Проверяет факт столкновения фигуры с элементами на поле
\return TRUE - столкновение произошло, FALSE - нет
*/
int check_attached();

/*!
\ingroup move
Проверяет возможен ли отскок фигуры от боковых границ поля при команде rotate()
*/
void rotation_prepare(int *is_shift_right, int *is_shift_left);

/*!
    \defgroup matrix Вспомогательные функции для работы с матрицами
    \brief Включают инициализацию игры, смену состояний и окончание
*/

/*!
\ingroup matrix
Динамически выделяет память под матрицу заданных размеров
\param[in] rows, cols Размер матрицы
\return Возвращает ссылку на выделенную память
В случае невыделения памяти происходит очистка частично выделенного и возвращение NULL
*/
int **create_matrix(int rows, int cols);

/*!
\ingroup matrix
Освобождает память по указателю matrix
\param[in] rows Количество строк матрицы
\param[matrix] Указатель на выделенную область памяти
*/
void remove_matrix(int **matrix, int rows);

/*!
\ingroup matrix
Построчно проверяет заполнение строк поля field. Заполненные строки удаляются
\return Количество удаленных заполненных строк
*/
int check_row();

/*!
\ingroup matrix
Удаляет из поля field указанную строку. Вышележащие строки точки смещаются на индекс ниже
\param[in] row Индекс сроки поля field
*/
void delete_row(int row);

/*!
\ingroup matrix
Выделяет память по навую матрицу размера size и копирует в нее данные из матрицы src
\param[in] src Указатель на матрицу, которую нужно скопировать
\param[in] size Размер скопированной матрицы
*/
int **copy_matrix(int **src, int size);

/*!
\ingroup matrix
Поворачивает матрицу из поля block
*/
void rotate_matrix();

/*!
\ingroup matrix
Отрисовывает фигуру block на поле field
*/
void pin_block();

/*!
\ingroup matrix
Удаляет фигуру block с поля field
*/
void unpin_block();

/*!
\ingroup matrix
Заполняет фигуру block согласно значению name
\param[in] block Указатель на матрицу для заполнения
\param[in] name Имя фигуры, которую нужно отрисовать
*/
void fill_block(int **block, int name);

/*!
    \defgroup test Костыль для тестирования  
    \brief Имитация mock-объекта game для тестирования
*/

/*!
\ingroup test
Возвращает ссылку на статический объект Game_t. Требуется для тестирования
*/
Game_t *game_data();

#endif