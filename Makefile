all : main
main : Treedir.c Treedir.h main.c
	gcc -Wall -o main Treedir.c
clean :
	rm -rf main