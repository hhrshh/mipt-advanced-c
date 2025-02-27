all: areafinder

# Компиляция программы
areafinder: main.o root.o integral.o func.o
	cc -o areafinder main.o root.o integral.o func.o

# Компиляция отдельных модулей
main.o: main.c
	cc -c -o main.o main.c
root.o: root.c
	cc -c -o root.o root.c
integral.o: integral.c
	cc -c -o integral.o integral.c
func.o: func.c
	cc -c -o func.o func.c

# Очистка проекта
del:
	rm *.o areafinder