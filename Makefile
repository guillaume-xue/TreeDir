CC = gcc
CFLAGS = -Wall
DEPS = Treedir.h
EXEC = main

all : $(EXEC)

Treedir.o : Treedir.c $(DEPS)
	$(CC) $(CFLAGS) -c Treedir.c

readFile.o : readFile.c $(DEPS)
	$(CC) $(CFLAGS) -c readFile.c

main.o : main.c $(DEPS)
	$(CC) $(CFLAGS) -c main.c

$(EXEC) : main.o Treedir.o readFile.o
	$(CC) $(CFLAGS) -o $(EXEC) Treedir.o readFile.o main.o

clean :
	rm -rf $(EXEC) *.o