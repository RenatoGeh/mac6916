CFLAGS = -pedantic -Wall -Wno-unused-result -std=c++11
DEBUG = false
LIBS = -ldai -lgmp -lgmpxx

ifeq ($(DEBUG), true)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

DEPS = nbayes.o

all: run

run: main.o $(DEPS)
	g++ $^ -o $@ $(LIBS)

main.o: main.cc nbayes.h
	g++ -c $(CFLAGS) $^

nbayes.o: nbayes.cc nbayes.h
	g++ -c $(CFLAGS) $^

.PHONY: clean clean_output
clean:
	rm *.out *.gch *.o run

clean_output:
	rm *.fg

