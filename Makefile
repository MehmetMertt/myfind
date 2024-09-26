.PHONY: all clean run

all: myfind

myfind: main.o
	g++ main.o -o myfind

main.o: main.cpp
	g++ -c main.cpp

ri: myfind
	./myfind -R -i ./folder1 test2.md test.md test4.md

ir: myfind
	./myfind -i -R ./folder1 test2.md test.md test4.md

n: myfind
	./myfind ./folder1 test2.md test.md test4.md

i: myfind
	./myfind -i ./folder1 test2.md test.md test4.md

r:	myfind
	./myfind -R ./folder1 test2.md test.md test4.md

clean:
	rm -f *.o myfind