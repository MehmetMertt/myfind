.PHONY: all clean

all: myfind

myfind: main.o
	g++ main.o -o myfind

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o myfind
