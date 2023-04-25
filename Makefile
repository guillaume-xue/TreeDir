all : main
Treedir.o : Treedir.c Treedir.h
	gcc -Wall -c Treedir.c
main.o : main.c Treedir.h
	gcc -Wall -c main.c
main : main.o Treedir.o
	gcc -Wall -o main Treedir.o main.o
clean :
	rm -rf main *.o