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
#define MENU_ITEMS 4 
#define MENU_COLORS 6
#define W 119
#define S 115

const char *choices[MENU_ITEMS] = {
        "1. Single Player",
        "2. PvP Mode",
        "3. Help",
        "4. Exit"
};

const char *colorMenu[MENU_COLORS] = {
    "RED",
    "YELLOW",
    "BLUE",
    "MAGENTA",
    "CYAN",
    "WHITE"
};

enum {LEFT = 1, UP, RIGHT, DOWN, STOP_GAME = KEY_F(10)};
enum {MAX_TAIL_SIZE = 100, START_TAIL_SIZE = 13, MAX_FOOD_SIZE = 20, FOOD_EXPIRE_SECONDS = 10, SEED_NUMBER = 3};
enum {SINGLE_PLAYER = 1, PVP, HELP, EXIT};

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

void initColor(void)
{
    start_color();                              // Включаем работу с цветами
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
}

void setColor(int objectType)
{
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    attroff(COLOR_PAIR(4));
    attroff(COLOR_PAIR(5));
    attroff(COLOR_PAIR(6));
    attroff(COLOR_PAIR(7));

    switch (objectType)
    {
        case 1:
        { // COLOR_RED
            attron(COLOR_PAIR(1));
            break;
        }
        case 2:
        { // COLOR_YELLOW
            attron(COLOR_PAIR(2));
            break;
        }
        case 3:
        { // COLOR_BLUE
            attron(COLOR_PAIR(3));
            break;
        }
        case 4:
        { // COLOR_MAGENTA
            attron(COLOR_PAIR(4));
            break;
        }
        case 5:
        { // COLOR_CYAN
            attron(COLOR_PAIR(5));
            break;
        }
        case 6:
        { // COLOR_WHITE
            attron(COLOR_PAIR(6));
            break;
        }
        case 7:
        { // COLOR_GREEN
            attron(COLOR_PAIR(7));
            break;
        }
    }
}

void drawMainMenu(void) 
{

    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);

    // Центрируем заголовок и меню
    int start_x = (max_x - 40) / 2; // Ширина ASCII-арта примерно 40 символов
    int start_y = (max_y - 17) / 2; // Высота меню примерно 15 строк

    // Рамка вокруг меню (используем ASCII-символы)
    attron(COLOR_PAIR(7)); // Зеленый цвет для рамки
    for (int i = start_x - 2; i <= start_x + 36; i++)
    {
        mvprintw(start_y - 2, i, "-");
        mvprintw(start_y + 16, i, "-");
    }
    for (int i = start_y - 1; i <= start_y + 16; i++)
    {
        mvprintw(i, start_x - 2, "|");
        mvprintw(i, start_x + 36, "|");
    }
    mvprintw(start_y - 2, start_x - 2, "+");
    mvprintw(start_y - 2, start_x + 36, "+");
    mvprintw(start_y + 16, start_x - 2, "+");
    mvprintw(start_y + 16, start_x + 36, "+");
    attroff(COLOR_PAIR(7));

    // ASCII-арт для заголовка "SnakeGO"
    attron(COLOR_PAIR(7)); // Зеленый цвет для змейки
    mvprintw(start_y, start_x + 4,     "   _____   _____   ___      ");
    mvprintw(start_y + 1, start_x + 4, "  / ____| / ____| / _ \\     ");
    mvprintw(start_y + 2, start_x + 4, " | (___  | |  __ | | | |    ");
    mvprintw(start_y + 3, start_x + 4, "  \\___ \\ | | |_ || | | |    ");
    mvprintw(start_y + 4, start_x + 4, "  ____) || |__| || |_| |    ");
    mvprintw(start_y + 5, start_x + 4, " |_____/  \\_____| \\___/     ");
    attroff(COLOR_PAIR(7));

    // Инструкция по управлению
    attron(COLOR_PAIR(6)); // Белый цвет для текста
    mvprintw(start_y + 7, start_x, "Use ARROW and W S, ENTER to select.");
    mvprintw(start_y + 8, start_x, "Press F10 to exit the game.");
    attroff(COLOR_PAIR(6));
}
//===============================================================================================================

void startMenu(WINDOW *win, int highlight1, int highlight2, const char **choices, int n_choices) 
{
    int max_y, max_x;
    getmaxyx(win, max_y, max_x); // Получаем размеры окна

    // Вычисляем начальные координаты для отрисовки меню по центру
    int start_y = (max_y - n_choices - 6) / 2;      // Центр по вертикали
    int start_x = (max_x - 42) / 2;             // Центр по горизонтали (20 - примерная ширина меню)

    for (int i = 0; i < n_choices; i++) 
    {
        if (highlight1 == i + 1 || highlight2 == i + 1)
        {
            wattron(win, A_REVERSE);
            if(highlight2 != 0)
            {
                wattroff(win, A_REVERSE);
                setColor(i + 1);
                mvwprintw(win, start_y + 7 + i, start_x + 18, "@***"); // Стрелочка перед выбранным пунктом
            }
        }
        if(highlight2 != 0)                                     // Для меню с выбором для двух
            setColor(i + 1);
        mvwprintw(win, start_y + 7 + i, start_x + 10, "%s", choices[i]);
        if (highlight1 == i + 1 || highlight2 == i + 1)
        {
            wattroff(win, A_REVERSE);
        }
    }
    wrefresh(win);
}

int navigationMenu(int key_pressed, int* highlight, int menu_items, int player)
{
    switch(key_pressed)
    {
        case 'w': // Используем символ 'w'
            if (player == 2) {
                *highlight = (*highlight == 1) ? menu_items : (*highlight - 1);
                return 0;
            }
            break;
        case KEY_UP:
            if (player == 1) {
                *highlight = (*highlight == 1) ? menu_items : (*highlight - 1);
                return 0;
            }
            break;
        case 's': // Используем символ 's'
            if (player == 2) {
                *highlight = (*highlight == menu_items) ? 1 : (*highlight + 1);
                return 0;
            }
            break;
        case KEY_DOWN:
            if (player == 1) {
                *highlight = (*highlight == menu_items) ? 1 : (*highlight + 1);
                return 0;
            }
            break;
        case 10: // Код клавиши Enter
            return *highlight;
        default:
            return 0;
    }
    return 0;
}

//===============================================================================================================

void initFood(struct food f[], size_t size)
{
    struct food init = {0};
    for(size_t i = 0; i < size; i++)
        f[i] = init;
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
    setColor(7);
    for(size_t i=0; i<number_seeds; i++)
        putFoodSeed(&f[i]);
}

void refreshFood(struct food f[], int nfood)
{
    setColor(7);
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
    head[i] = (snake_t*)malloc(sizeof(snake_t));
    tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head[i], x, y);
    head[i]->tail = tail; // прикрепляем к голове хвост
    head[i]->tsize = size + 1;
    head[i]->controls = default_controls[i];
}



/*
 Движение головы с учетом текущего направления движения
 */
void go(struct snake_t *head, int color)
{
    setColor(color);
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
    setColor(color);
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

// Показывает результаты, сколько было съедено семян
void showScores(struct snake_t *head, int snake, int color)
{
    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);
    setColor(color);
    if(!snake)
        mvprintw(0, 0, "Snake1 - %lu", head->tsize - 4);
    else
        mvprintw(0, max_x - 11, "Snake2 - %lu", head->tsize - 4);
    refresh();
}

// Вынести тело цикла while из int main() в отдельную функцию update
void update(struct snake_t *head, struct food f[], const int32_t key_pressed, int ai, int color)
{
    if(ai)                                                      // для каждого плеера свое управление
        autoChangeDirection(head, f, SEED_NUMBER);
    else 
        if(checkDirection(head, key_pressed)) 
            changeDirection(head, key_pressed);
            
    clock_t begin = clock();
    go(head, color);
    goTail(head, color);

    if(key_pressed != ERR && checkDirection(head, key_pressed)) // нажата ли клавиша и если вдижение змейки и направление нажатия корректны
        changeDirection(head, key_pressed);

    if(haveEat(head, food))                                     // Проверка на съедение
    addTail(head);                                              // Добавляем + 1 к хвосту

    refreshFood(food, SEED_NUMBER);                             // Обновляем еду
    repairSeed(food, SEED_NUMBER, head);                        // Определяем корректность установки зерен
                                                                // Если не правильно обновляем зерна
    refresh();                                                  // Обновление экрана, вывели кадр анимации
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY)
    {}
}

void showHelpMenu(void)
{
    clear(); // Clear the screen
    attron(COLOR_PAIR(7));
    mvprintw(1, 1, "=== Control Instructions ===");
    mvprintw(3, 1, "Menu Controls:");
    mvprintw(4, 1, "Player 1: UP, DOWN, Enter");
    mvprintw(5, 1, "Player 2: W, S, Enter");
    mvprintw(7, 1, "In-Game Controls:");
    mvprintw(8, 1, "Player 1: UP, DOWN, LEFT, RIGHT");
    mvprintw(9, 1, "Player 2: W, S, A, D");
    mvprintw(11, 1, "General Keys:");
    mvprintw(12, 1, "F10: Exit to Main Menu");
    mvprintw(14, 1, "=== Game Rules ===");
    mvprintw(15, 1, "1. Eat food ($) to grow.");
    mvprintw(16, 1, "2. Avoid collisions with your tail and walls.");
    mvprintw(17, 1, "3. In PvP, the player with the highest score wins.");
    mvprintw(19, 1, "Press any key to return to the menu...");
    refresh();
    getch(); // возврат в главное меню
    attroff(COLOR_PAIR(7));
}

//========================================================================

int main()
{
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Откдючаем line buffering
    noecho();             // Отключаем echo() режим при вызове getch
    curs_set(FALSE);      // Отключаем курсор
    initColor();          // Инизиализируем цвета для меню
    
    int max_x = getmaxx(stdscr); // определяем координаты max X
    int max_y = getmaxy(stdscr); // определяем координаты max Y
    int highlight1 = 1, highlight2 = 2;    // Переменная для выбора
    int key_pressed = 0;
    int choice = 0;
    int color1 = 0, color2 = 2;

  //=====================Меню=========================================================================================================//
    MAIN_MENU:
    
    while(1)                                                     // Главное меню, выбор режима игры.
    {
        drawMainMenu();
        startMenu(stdscr, highlight1, 0, choices, MENU_ITEMS);                          // Отрисовываем меню
        key_pressed = getch();                                                          // Считываем нажатую клавишу
        choice = navigationMenu(key_pressed, &highlight1, MENU_ITEMS, 1);               // Навигация и выбор элемента
        if(key_pressed == STOP_GAME)
        {
            endwin();                                                               // Завершаем режим curses mod
            return 0;
        }
        if (choice != 0)                                                                // Если пункт выбран выходим в следуюищее меню
            break;
    }

    switch (choice)
    {
    case SINGLE_PLAYER:
        highlight1 = 1;                                                                 // Первый элемент
        while(1)                                                        
        {
            clear();                                                                    // Очищаем экран
            drawMainMenu();
            startMenu(stdscr, highlight1, highlight2, colorMenu, MENU_COLORS);          // Отрисовываем меню
            key_pressed = getch();
            if(key_pressed == STOP_GAME)
            {
                endwin();                                                               // Завершаем режим curses mod
                return 0;
            }                                                    // Считываем нажатую клавишу
            color1 = navigationMenu(key_pressed, &highlight1, MENU_COLORS, 1);          // Навигация и выбор элемента
            color2 = navigationMenu(key_pressed, &highlight2, MENU_COLORS, 2);          // Навигация и выбор элемента
            if(color1 != 0)                                                             // Если пункт выбран выходим в следуюищее меню
                break;
        }
        break;
    case PVP:
        highlight1 = 1;                                                                 // Второй элемент
        while(1)                                 
        {
            clear();                                                                    // Очищаем экран
            drawMainMenu();
            startMenu(stdscr, highlight1, highlight2, colorMenu, MENU_COLORS);          // Отрисовываем меню
            key_pressed = getch();
            if(key_pressed == STOP_GAME)
            {
                endwin();                                                               // Завершаем режим curses mod
                return 0;
            }                                                      // Считываем нажатую клавишу
            color1 = navigationMenu(key_pressed, &highlight1, MENU_COLORS, 1);          // Навигация и выбор элемента
            color2 = navigationMenu(key_pressed, &highlight2, MENU_COLORS, 2);
            if(color1 != 0)                                                             // Если пункт выбран выходим в следуюищее меню
                break;
        }
        break;
    case HELP:                                                            // Третьий элемент
            showHelpMenu();
            clear();
            goto MAIN_MENU;
        break;
    case EXIT:
            endwin();
            return 0;
        break;
    default:
        break;
    }
    //=====================Меню=========================================================================================================//
    snake_t* snakes[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
        initSnake(snakes,START_TAIL_SIZE, 10 + i * 10, 10 + i * 10, i);

    timeout(0);                                                             // Отключаем таймаут после нажатия клавиши в цикле
    initFood(food, MAX_FOOD_SIZE);

    clear();                                                                // Очищаем экран
    putFood(food, SEED_NUMBER);                                             // Кладем зерна
    key_pressed = 0;
    int game_over = 0;
    while(key_pressed != STOP_GAME && !game_over)
    {
        key_pressed = getch();                                              // Считываем клавишу
        for (int i = 0; i < PLAYERS; ++i)
        {  
            showScores(snakes[i], i, i ? color1 : color2);
            update(snakes[i], food, key_pressed, choice == 2 ? 0 : i, i ? color1 : color2);
            if(isCrush(snakes[i]))                                          // Проверяем, если голова пересекла тело
            {
                mvprintw(10, 35, "GAME OVER");
                mvprintw(11, 34, "SNAKE%d WIN!", !i + 1);
                refresh();
                snake_getch_delay(2000);                                    // Задержка для прочтения GAME OVER!
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
    endwin();                                                               // Завершаем режим curses mod
    return 0;
}