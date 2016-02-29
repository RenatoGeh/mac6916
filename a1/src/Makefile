CFLAGS = -pedantic -Wall -Wno-unused-result -std=c++11
DEBUG = false

ifeq ($(DEBUG), true)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

DEPS = distribution.o variable.o

all: run

run: main.o $(DEPS)
	g++ $^ -o $@ $(LINKS)

main.o: main.cc
	g++ -c $(CFLAGS) $^

distribution.o: distribution.cc distribution.h variable.h
	g++ -c $(CFLAGS) $^

variable.o:
	g++ -c $(CFLAGS) $^

.PHONY: clean
clean:
	rm *.out *.gch *.o run

