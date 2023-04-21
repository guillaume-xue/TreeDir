all : Treedir
Treedir : Treedir.c Treedir.h
	gcc -Wall -o Treedir Treedir.c
clean :
	rm -rf prog