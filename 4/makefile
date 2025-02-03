all: snake

snake: snake.o
	gcc -o snake snake.o -lncurses
rep:
	gcc -Wall -pg snake.c -lncurses -o snake;./snake; gprof -p -b ./snake gmon.out > reports.txt
mem:
	valgrind --leak-check=full ./snake
del:
	rm snake reports.txt snake.o gmon.out