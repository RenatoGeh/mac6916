CFLAGS = -pedantic -Wall -Wno-unused-result -std=c++11
DEBUG = false

ifeq ($(DEBUG), true)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

DEPS = variable.o bayes.o factor.o

all: run

run: main.o $(DEPS)
	g++ $^ -o $@

main.o: main.cc
	g++ -c $(CFLAGS) $^

variable.o: variable.cc variable.h
	g++ -c $(CFLAGS) $^

bayes.o: bayes.cc bayes.h factor.h variable.h
	g++ -c $(CFLAGS) $^

factor.o: factor.cc factor.h
	g++ -c $(CFLAGS) $^

.PHONY: clean
clean:
	rm *.out *.gch *.o run

