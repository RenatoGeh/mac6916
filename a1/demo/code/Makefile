# Makefile
EXE	= query alarm
OBJS	= bison.o lex.o main.o expression.o problogic.o
CC	= gcc
CFLAGS	= -Wall -std=c11

all:		query alarm
		@echo "Run 'alarm' or './query alarm.model' for examples."

query:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o query

alarm:		alarm.o expression.o problogic.o
		$(CC) $(CFLAGS) alarm.o expression.o problogic.o -o alarm

lex.o:		lex.c
		$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:		model.lex 
		flex model.lex
		cp lex.yy.c lex.c

bison.o:	bison.c
		$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	model.y
		bison -d model.y
		cp model.tab.c bison.c
		cmp -s model.tab.h tok.h || cp model.tab.h tok.h

main.o:		main.c
		$(CC) $(CFLAGS) -c main.c -o main.o

alarm.o:	alarm.c
		$(CC) $(CFLAGS) -c alarm.c -o alarm.o


problogic.o: 	problogic.c 
		$(CC) $(CFLAGS) -c problogic.c -o problogic.o

expression.o: 	expression.c 
		$(CC) $(CFLAGS) -c expression.c -o expression.o


lex.o main.o		: tok.h

clean:
	rm -f *.o *~ $(EXE) lex.c lex.yy.c bison.c tok.h model.tab.c model.tab.h model.output
