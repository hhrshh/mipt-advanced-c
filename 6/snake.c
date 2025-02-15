#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y 2
#define CONTROLS 2
const double DELAY = 0.05;
#define PLAYERS  2
#define MENU_ITEMS 3 

const char *choices[MENU_ITEMS] = {
    "Single playr",
    "PVP",
    "About"
};

enum {LEFT = 1, UP, RIGHT, DOWN, STOP_GAME = KEY_F(10)};
enum {MAX_TAIL_SIZE = 100, START_TAIL_SIZE = 3, MAX_FOOD_SIZE = 20, FOOD_EXPIRE_SECONDS = 10, SEED_NUMBER = 3};


// Здесь храним коды управления змейкой
struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons;

struct control_buttons default_controls[CONTROLS] = {{KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
                                                    {'s', 'w', 'a', 'd'}};
/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons controls;
} snake_t;

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

struct food
{
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food[MAX_FOOD_SIZE];

void initColor(int color_snake_1, int color_snake_2)
{
    start_color();                              // Включаем работу с цветами
    init_pair(1, color_snake_1, COLOR_BLACK);
    init_pair(2, color_snake_2, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
}

//============================================================================
void startMenu(WINDOW *win, int highlight, const char **choices, int n_choices) 
{
    int max_y, max_x;
    getmaxyx(win, max_y, max_x); // Получаем размеры окна

    // Вычисляем начальные координаты для отрисовки меню по центру
    int start_y = (max_y - n_choices) / 2; // Центр по вертикали
    int start_x = (max_x - 20) / 2;        // Центр по горизонтали (20 - примерная ширина меню)

    for (int i = 0; i < n_choices; i++) 
    {
        if (highlight == i + 1)
        { // Если пункт выделен
            wattron(win, A_REVERSE); // Включаем инверсию цвета
            mvwprintw(win, start_y + i, start_x, "%s", choices[i]);
            wattroff(win, A_REVERSE); // Выключаем инверсию цвета
        }
        else 
        {
            mvwprintw(win, start_y + i, start_x, "%s", choices[i]);
        }
    }
    wrefresh(win);
}

int navigationMenu(int key_pressed, int highlight)
{
    switch(key_pressed)
    {
        case KEY_UP:
            if(highlight == 1)
                return MENU_ITEMS; // Переход вниз, если достигнут верх
            else
                return --highlight;
            break;
        case KEY_DOWN:
            if(highlight == MENU_ITEMS)
                return 1;          // Переход вверх, если достигнут низ 
            else 
                return ++highlight;
            break;
        case 10: // Код клавиши Enter
            return 0;
            break;
        default:
            return 0;
            break;
    }

}

//============================================================================

void initFood(struct food f[], size_t size)
{
    struct food init = {0};
    for(size_t i = 0; i < size; i++)
        f[i] = init;
}



void setColor(int objectType)
{
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    switch (objectType)
    {
        case 1:{ // SNAKE1
            attron(COLOR_PAIR(1));
            break;
        }
        case 2:{ // SNAKE2
            attron(COLOR_PAIR(2));
            break;
        }
        case 3:{ // FOOD
            attron(COLOR_PAIR(3));
            break;
        }
    }
}

/*
 Обновить/разместить текущее зерно на поле
 */
void putFoodSeed(struct food *fp)
{
    int max_x=0, max_y=0;
    char spoint[2] = {0};
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(fp->y, fp->x, " ");
    fp->x = rand() % (max_x - 1);
    fp->y = rand() % (max_y - 2) + 1; //Не занимаем верхнюю строку
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    spoint[0] = fp->point;
    mvprintw(fp->y, fp->x, "%s", spoint);
}

/*
 Разместить еду на поле
 */
void putFood(struct food f[], size_t number_seeds)
{
    setColor(3);
    for(size_t i=0; i<number_seeds; i++)
        putFoodSeed(&f[i]);
}

void refreshFood(struct food f[], int nfood)
{
    setColor(3);
    for(size_t i=0; i < nfood; i++)
        if(f[i].put_time)
            if(!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS)
                putFoodSeed(&f[i]);
}

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t = {0,0};
    for(size_t i = 0; i < size; i++)
        t[i] = init_t;
}

void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head[], size_t size, int x, int y,int i)
{
    head[i]    = (snake_t*)malloc(sizeof(snake_t));
    tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head[i], x, y);
    head[i]->tail     = tail; // прикрепляем к голове хвост
    head[i]->tsize    = size + 1;
    head[i]->controls = default_controls[i];
    //~ head[i]->controls = default_controls[0];
}



/*
 Движение головы с учетом текущего направления движения
 */
void go(struct snake_t *head, int color)
{
    setColor(color + 1);
    char ch = '@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction)
    {
        case LEFT:
            if(head->x <= 0) // Циклическое движение, чтобы не уходить за пределы экрана
                head->x = max_x;
            mvprintw(head->y, --(head->x), "%c", ch);
        break;
        case RIGHT:
            if(head->x >= max_x) // Циклическое движение, чтобы не уходить за пределы экрана
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", ch);
        break;
        case UP:
            if(head->y <= 1) // Циклическое движение, чтобы не уходить за пределы экрана
                head->y = max_y;
            mvprintw(--(head->y), head->x, "%c", ch);
        break;
        case DOWN:
            if(head->y >= max_y) // Циклическое движение, чтобы не уходить за пределы экрана
                head->y = 0;
            mvprintw(++(head->y), head->x, "%c", ch);
        break;
        default:
        break;
    }
    refresh();
}

int checkDirection(snake_t* snake, int32_t key)
{
    for(size_t i = 0; i < CONTROLS; ++i)
    {
        if(snake->direction == DOWN && key == default_controls[i].up)
            return 0;
        else if(snake->direction == UP && key == default_controls[i].down)
            return 0;
        else if(snake->direction == RIGHT && key == default_controls[i].left)
            return 0;
        else if(snake->direction == LEFT && key == default_controls[i].right)
            return 0;   
    }
    return 1;
}

void changeDirection(struct snake_t* snake, const int32_t key)
{
    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(struct snake_t *head, int color)
{
    setColor(color + 1);
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if( head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

int isCrush(snake_t *head)
{
    for (size_t i = 1; i < head->tsize; i++)
        if(head->x == head->tail[i].x && head->y == head->tail[i].y)
            return 1;
    return 0;
}

int snake_getch_delay(int millis)
{
    clock_t start_time = clock();
    while ((clock() - start_time) * 1000 / CLOCKS_PER_SEC < millis)
    {
        int ch = getch();
        if (ch != ERR) // Если была нажата клавиша
        {
            return ch;
        }
    }
    return ERR; // Если клавиша не была нажата
}



//Проверка того, является ли какое-то из зерен съеденным,
_Bool haveEat(struct snake_t *head, struct food f[])
{
    for(size_t i = 0; i < MAX_FOOD_SIZE; ++i)
        if(head->x == f[i].x && head->y == f[i].y && f[i].enable)
        {
            f[i].enable = 0;
            return true;
        }
    return false;
}

/*
 Увеличение хвоста на 1 элемент
 */

void addTail(struct snake_t *head)
{
    head->tsize++;
}


void repairSeed(struct food f[], size_t nfood, struct snake_t *head)
{
    for(size_t i = 0; i < head->tsize; ++i)
        for(size_t j = 0; j < nfood; ++j)
        {
            if(f[j].x == head->tail[i].x && f[j].y == head->tail[i].y && f[i].enable) // если зерна совпадают со змейкой
                refreshFood(f, SEED_NUMBER);                                       // обновляем еду   
        }

    for(size_t i = 0; i < nfood; ++i)
        for(size_t j = 0; j < nfood; ++j)
        {
            if(i!=j && f[i].enable && f[j].enable && f[j].x == f[i].x && f[j].y == f[i].y && f[i].enable) // Если два зерна на одной точке
                refreshFood(f, SEED_NUMBER);                                      // обновляем еду
        }
}

// Добавление ИИ   
int distance(const snake_t snake, const struct food food) {   // вычисляет количество ходов до еды
    return (abs(snake.x - food.x) + abs(snake.y - food.y));
}
// Для автоизменения направления напишем функцию
// Она определяет ближайшую к себе еду и движется по направлению к ней

void autoChangeDirection(snake_t *snake, struct food food[], int foodSize)
{
    int pointer = 0;
    for (int i = 1; i < foodSize; i++) {   // ищем ближайшую еду
        pointer = (distance(*snake, food[i]) < distance(*snake, food[pointer])) ? i : pointer;
    }
    if ((snake->direction == RIGHT || snake->direction == LEFT) &&
        (snake->y != food[pointer].y)) {  // горизонтальное движение
        snake->direction = (food[pointer].y > snake->y) ? DOWN : UP;
    } else if ((snake->direction == DOWN || snake->direction == UP) &&
               (snake->x != food[pointer].x)) {  // вертикальное движение
        snake->direction = (food[pointer].x > snake->x) ? RIGHT : LEFT;
    }
}

// Вынести тело цикла while из int main() в отдельную функцию update
void update(struct snake_t *head, struct food f[], const int32_t key_pressed, int ai, int color)
{
    if(ai)                                                   // для каждого плеера свое управление
        autoChangeDirection(head, f, SEED_NUMBER);
    else 
        if(checkDirection(head, key_pressed)) 
            changeDirection(head, key_pressed);

    clock_t begin = clock();
    go(head, color);
    goTail(head, color);

    if(key_pressed != ERR && checkDirection(head, key_pressed)) // нажата ли клавиша и если вдижение змейки и направление нажатия корректны
        changeDirection(head, key_pressed);

    if (haveEat(head, food))                                // Проверка на съедение
    addTail(head);                                          // Добавляем + 1 к хвосту

    refreshFood(food, SEED_NUMBER);                         // Обновляем еду
    repairSeed(food, SEED_NUMBER, head);                    // Определяем корректность установки зерен
                                                            // Если не правильно обновляем зерна
    refresh();                                              // Обновление экрана, вывели кадр анимации
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY)
    {}
}

//========================================================================

int main()
{
    snake_t* snakes[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
        initSnake(snakes,START_TAIL_SIZE, 10 + i * 10, 10 + i * 10, i);
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Откдючаем line buffering
    noecho();             // Отключаем echo() режим при вызове getch
    curs_set(FALSE);      // Отключаем курсор
    mvprintw(0, 1, "Use arrows for control. Press 'F10' for EXIT. Enter to select:");
    initColor(COLOR_RED, COLOR_BLUE);          // Инизиализируем цвета для меню
    int highlight = 1;    // Переменная для выбора
    int key_pressed = 0;
    int choice = 0;
    while(1)
    { 
        startMenu(stdscr, highlight, choices, MENU_ITEMS);         // Отрисовываем меню
        key_pressed = getch();
        switch (key_pressed) {
            case KEY_UP:
                if (highlight == 1) {
                    highlight = MENU_ITEMS; // Переход вниз, если достигнут верх
                } else {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight == MENU_ITEMS) {
                    highlight = 1; // Переход вверх, если достигнут низ
                } else {
                    highlight++;
                }
                break;
            case 10: // Код клавиши Enter
                choice = highlight;
                break;
            default:
                break;
        }
        if (choice != 0) // Если пункт выбран
            break;
    }                              

    timeout(0);           // Отключаем таймаут после нажатия клавиши в цикле
    initFood(food, MAX_FOOD_SIZE);

    clear();                                   // Очищаем экран
    putFood(food, SEED_NUMBER);                // Кладем зерна
    key_pressed = 0;
    int game_over = 0;
    while(key_pressed != STOP_GAME && !game_over)
    {
        key_pressed = getch();                                               // Считываем клавишу
        for (int i = 0; i < PLAYERS; i++)
        {
                
            update(snakes[i], food, key_pressed, choice == 2 ? 0 : i, i);
            if(isCrush(snakes[i]))                                           // Проверяем, если голова пересекла тело
            {
                mvprintw(10, 35, "GAME OVER!");
                refresh();
                snake_getch_delay(2000);                                     // Задержка для прочтения GAME OVER!
                game_over = 1;
                break;
            }
        }  
    }

    for (int i = 0; i < PLAYERS; i++)
    {
        free(snakes[i]->tail);
        free(snakes[i]);
    }
    endwin();                 // Завершаем режим curses mod
    return 0;
}