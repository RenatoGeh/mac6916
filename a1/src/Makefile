CFLAGS = -pedantic -Wall -Wno-unused-result -std=c++11
DEBUG = false

ifeq ($(DEBUG), true)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

DEPS = jointdist.o

all: run

run: main.o $(DEPS)
	g++ $^ -o $@

main.o: main.cc jointdist.h
	g++ -c $(CFLAGS) $^

jointdist.o: jointdist.cc jointdist.h
	g++ -c $(CFLAGS) $^

.PHONY: clean
clean:
	rm *.out *.gch *.o run

