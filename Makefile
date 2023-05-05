CC = gcc
CFLAGS = -Wall
DEPS = Treedir.h
EXEC = Treedir
FOL = out/
MAIN = main/

all : $(EXEC)

out :
	mkdir $(FOL)

Treedir.o : $(MAIN)Treedir.c $(MAIN)$(DEPS)
	$(CC) $(CFLAGS) -c $(MAIN)Treedir.c

readFile.o : $(MAIN)readFile.c $(MAIN)$(DEPS)
	$(CC) $(CFLAGS) -c $(MAIN)readFile.c

main.o : $(MAIN)main.c $(MAIN)$(DEPS)
	$(CC) $(CFLAGS) -c $(MAIN)main.c

move:
	mv *.o out

$(EXEC) : out main.o Treedir.o readFile.o move
	$(CC) $(CFLAGS) -o $(EXEC) $(FOL)Treedir.o $(FOL)readFile.o $(FOL)main.o

clean :
	rm -rf $(EXEC) $(FOL)